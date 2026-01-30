#include "monitors/RAMMonitor.h"
#include <iostream>

RAMMonitor::RAMMonitor()
    : m_initialized(false)
{
    m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
}

RAMMonitor::~RAMMonitor() {
    // No cleanup needed for memory monitoring
}

bool RAMMonitor::initialize() {
    if (m_initialized) {
        return true;
    }

    // Try to get initial memory status
    if (!GlobalMemoryStatusEx(&m_memInfo)) {
        std::cerr << "Failed to get memory status. Error: " << GetLastError() << std::endl;
        return false;
    }

    m_initialized = true;
    return true;
}

void RAMMonitor::update() {
    if (!m_initialized) {
        return;
    }

    collectData();
}

uint64_t RAMMonitor::getTotalPhysicalMemory() const {
    return m_memInfo.ullTotalPhys;
}

uint64_t RAMMonitor::getAvailablePhysicalMemory() const {
    return m_memInfo.ullAvailPhys;
}

uint64_t RAMMonitor::getUsedPhysicalMemory() const {
    return m_memInfo.ullTotalPhys - m_memInfo.ullAvailPhys;
}

double RAMMonitor::getMemoryUsagePercent() const {
    return static_cast<double>(m_memInfo.dwMemoryLoad);
}

uint64_t RAMMonitor::getTotalVirtualMemory() const {
    return m_memInfo.ullTotalPageFile;
}

uint64_t RAMMonitor::getAvailableVirtualMemory() const {
    return m_memInfo.ullAvailPageFile;
}

uint64_t RAMMonitor::getUsedVirtualMemory() const {
    return m_memInfo.ullTotalPageFile - m_memInfo.ullAvailPageFile;
}

bool RAMMonitor::isInitialized() const {
    return m_initialized;
}

void RAMMonitor::collectData() {
    // Update memory information
    if (!GlobalMemoryStatusEx(&m_memInfo)) {
        std::cerr << "Failed to collect memory data. Error: " << GetLastError() << std::endl;
    }
}
