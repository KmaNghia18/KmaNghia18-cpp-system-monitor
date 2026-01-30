#include "monitors/DiskMonitor.h"
#include <iostream>

#pragma comment(lib, "pdh.lib")

DiskMonitor::DiskMonitor()
    : m_query(nullptr)
    , m_initialized(false)
{
}

DiskMonitor::~DiskMonitor() {
    // Clean up PDH resources
    for (auto& counter : m_speedCounters) {
        if (counter.readCounter) {
            PdhRemoveCounter(counter.readCounter);
        }
        if (counter.writeCounter) {
            PdhRemoveCounter(counter.writeCounter);
        }
    }
    
    if (m_query) {
        PdhCloseQuery(m_query);
    }
}

bool DiskMonitor::initialize() {
    if (m_initialized) {
        return true;
    }

    // Enumerate available drives
    enumerateDrives();
    
    if (m_disks.empty()) {
        std::cerr << "No disk drives found" << std::endl;
        return false;
    }

    // Create PDH query for speed monitoring
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &m_query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to open PDH query for disk monitoring. Error: " << status << std::endl;
        return false;
    }

    // Setup speed counters
    if (!setupSpeedCounters()) {
        std::cerr << "Failed to setup disk speed counters" << std::endl;
    }

    // Initial data collection
    collectSpaceInfo();
    
    // Collect initial PDH data
    if (m_query) {
        PdhCollectQueryData(m_query);
    }

    m_initialized = true;
    return true;
}

void DiskMonitor::update() {
    if (!m_initialized) {
        return;
    }

    collectSpaceInfo();
    collectSpeedInfo();
}

const std::vector<DiskInfo>& DiskMonitor::getDiskInfo() const {
    return m_disks;
}

const DiskInfo* DiskMonitor::getDriveInfo(const std::wstring& driveLetter) const {
    for (const auto& disk : m_disks) {
        if (disk.driveLetter == driveLetter) {
            return &disk;
        }
    }
    return nullptr;
}

int DiskMonitor::getDriveCount() const {
    return static_cast<int>(m_disks.size());
}

bool DiskMonitor::isInitialized() const {
    return m_initialized;
}

void DiskMonitor::enumerateDrives() {
    m_disks.clear();

    // Get all logical drives
    DWORD drives = GetLogicalDrives();
    
    for (int i = 0; i < 26; i++) {
        if (drives & (1 << i)) {
            wchar_t driveLetter[4];
            swprintf_s(driveLetter, L"%c:", L'A' + i);
            
            // Check if it's a fixed drive (hard disk)
            UINT driveType = GetDriveTypeW(driveLetter);
            if (driveType == DRIVE_FIXED || driveType == DRIVE_REMOVABLE) {
                DiskInfo info;
                info.driveLetter = driveLetter;
                info.totalSpace = 0;
                info.freeSpace = 0;
                info.usedSpace = 0;
                info.usagePercent = 0.0;
                info.readSpeed = 0.0;
                info.writeSpeed = 0.0;
                
                // Get volume name and file system
                wchar_t volumeName[MAX_PATH + 1] = {0};
                wchar_t fileSystem[MAX_PATH + 1] = {0};
                GetVolumeInformationW(driveLetter, volumeName, MAX_PATH, nullptr, nullptr, nullptr, fileSystem, MAX_PATH);
                
                info.volumeName = volumeName;
                info.fileSystem = fileSystem;
                
                m_disks.push_back(info);
            }
        }
    }
}

void DiskMonitor::collectSpaceInfo() {
    for (auto& disk : m_disks) {
        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
        
        if (GetDiskFreeSpaceExW(disk.driveLetter.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
            disk.totalSpace = totalBytes.QuadPart;
            disk.freeSpace = totalFreeBytes.QuadPart;
            disk.usedSpace = disk.totalSpace - disk.freeSpace;
            
            if (disk.totalSpace > 0) {
                disk.usagePercent = (static_cast<double>(disk.usedSpace) / static_cast<double>(disk.totalSpace)) * 100.0;
            }
        }
    }
}

void DiskMonitor::collectSpeedInfo() {
    if (!m_query) {
        return;
    }

    // Collect query data
    PDH_STATUS status = PdhCollectQueryData(m_query);
    if (status != ERROR_SUCCESS) {
        return;
    }

    // Get read/write speeds for each disk
    for (size_t i = 0; i < m_speedCounters.size() && i < m_disks.size(); i++) {
        PDH_FMT_COUNTERVALUE counterValue;
        
        // Read speed
        if (m_speedCounters[i].readCounter) {
            status = PdhGetFormattedCounterValue(m_speedCounters[i].readCounter, PDH_FMT_DOUBLE, nullptr, &counterValue);
            if (status == ERROR_SUCCESS) {
                m_disks[i].readSpeed = counterValue.doubleValue;
            }
        }
        
        // Write speed
        if (m_speedCounters[i].writeCounter) {
            status = PdhGetFormattedCounterValue(m_speedCounters[i].writeCounter, PDH_FMT_DOUBLE, nullptr, &counterValue);
            if (status == ERROR_SUCCESS) {
                m_disks[i].writeSpeed = counterValue.doubleValue;
            }
        }
    }
}

bool DiskMonitor::setupSpeedCounters() {
    if (!m_query) {
        return false;
    }

    m_speedCounters.clear();

    for (const auto& disk : m_disks) {
        SpeedCounters counters;
        counters.readCounter = nullptr;
        counters.writeCounter = nullptr;
        
        // Try to find the physical disk number for this drive
        // Note: This is a simplified approach. In reality, mapping drive letters to physical disks is complex.
        // We'll use _Total for demonstration, or try individual disk instances
        
        std::wstring diskName = disk.driveLetter.substr(0, 1); // Just the letter (C, D, etc.)
        counters.diskName = diskName;
        
        // Try to add read counter
        std::wstring readCounterPath = L"\\LogicalDisk(" + diskName + L":)\\Disk Read Bytes/sec";
        PDH_STATUS status = PdhAddCounter(m_query, readCounterPath.c_str(), 0, &counters.readCounter);
        if (status != ERROR_SUCCESS) {
            counters.readCounter = nullptr;
        }
        
        // Try to add write counter
        std::wstring writeCounterPath = L"\\LogicalDisk(" + diskName + L":)\\Disk Write Bytes/sec";
        status = PdhAddCounter(m_query, writeCounterPath.c_str(), 0, &counters.writeCounter);
        if (status != ERROR_SUCCESS) {
            counters.writeCounter = nullptr;
        }
        
        m_speedCounters.push_back(counters);
    }

    return !m_speedCounters.empty();
}
