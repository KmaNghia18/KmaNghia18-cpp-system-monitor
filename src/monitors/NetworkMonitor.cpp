#include "monitors/NetworkMonitor.h"
#include "utils/SystemUtils.h"
#include <iostream>

NetworkMonitor::NetworkMonitor()
    : m_initialized(false)
    , m_lastUpdateTime(0)
{
}

NetworkMonitor::~NetworkMonitor() {
    // No cleanup needed
}

bool NetworkMonitor::initialize() {
    if (m_initialized) {
        return true;
    }

    // Collect initial data
    collectData();
    
    // Initialize previous stats
    m_previousStats.resize(m_interfaces.size());
    for (size_t i = 0; i < m_interfaces.size(); i++) {
        m_previousStats[i].bytesReceived = m_interfaces[i].bytesReceived;
        m_previousStats[i].bytesSent = m_interfaces[i].bytesSent;
        m_previousStats[i].timestamp = GetTickCount();
    }
    
    m_lastUpdateTime = GetTickCount();
    m_initialized = true;
    return true;
}

void NetworkMonitor::update() {
    if (!m_initialized) {
        return;
    }

    collectData();
    calculateSpeeds();
}

const std::vector<NetworkInterfaceInfo>& NetworkMonitor::getInterfaceInfo() const {
    return m_interfaces;
}

double NetworkMonitor::getTotalDownloadSpeed() const {
    double total = 0.0;
    for (const auto& iface : m_interfaces) {
        if (iface.isActive) {
            total += iface.downloadSpeed;
        }
    }
    return total;
}

double NetworkMonitor::getTotalUploadSpeed() const {
    double total = 0.0;
    for (const auto& iface : m_interfaces) {
        if (iface.isActive) {
            total += iface.uploadSpeed;
        }
    }
    return total;
}

uint64_t NetworkMonitor::getTotalBytesDownloaded() const {
    uint64_t total = 0;
    for (const auto& iface : m_interfaces) {
        total += iface.bytesReceived;
    }
    return total;
}

uint64_t NetworkMonitor::getTotalBytesUploaded() const {
    uint64_t total = 0;
    for (const auto& iface : m_interfaces) {
        total += iface.bytesSent;
    }
    return total;
}

bool NetworkMonitor::isInitialized() const {
    return m_initialized;
}

void NetworkMonitor::collectData() {
    m_interfaces.clear();

    // Get the table of network interfaces (use GetIfTable for Ming compatibility)
    ULONG bufferSize = 0;
    GetIfTable(nullptr, &bufferSize, FALSE);
    
    MIB_IFTABLE* ifTable = (MIB_IFTABLE*)malloc(bufferSize);
    if (!ifTable) {
        return;
    }
    
    DWORD result = GetIfTable(ifTable, &bufferSize, FALSE);
    
    if (result != NO_ERROR) {
        std::cerr << "Failed to get network interface table. Error: " << result << std::endl;
        free(ifTable);
        return;
    }

    // Iterate through all interfaces
    for (DWORD i = 0; i < ifTable->dwNumEntries; i++) {
        MIB_IFROW& row = ifTable->table[i];
        
        // Filter out loopback
        if (row.dwType == MIB_IF_TYPE_LOOPBACK) {
            continue;
        }

        NetworkInterfaceInfo info;
        
        // Convert description to wstring
        char descBuffer[MAXLEN_IFDESCR + 1] = {0};
        memcpy(descBuffer, row.bDescr, row.dwDescrLen);
        info.description = SystemUtils::stringToWstring(descBuffer);
        info.name = info.description;  // Use description as name
        
        info.bytesReceived = row.dwInOctets;
        info.bytesSent = row.dwOutOctets;
        info.downloadSpeed = 0.0;
        info.uploadSpeed = 0.0;
        info.isActive = (row.dwOperStatus == MIB_IF_OPER_STATUS_OPERATIONAL);
        
        m_interfaces.push_back(info);
    }

    free(ifTable);
}

void NetworkMonitor::calculateSpeeds() {
    DWORD currentTime = GetTickCount();
    double timeDelta = (currentTime - m_lastUpdateTime) / 1000.0; // Convert to seconds
    
    if (timeDelta <= 0.0) {
        return;  // Avoid division by zero
    }

    // Resize previous stats if interface count changed
    if (m_previousStats.size() != m_interfaces.size()) {
        m_previousStats.resize(m_interfaces.size());
        for (size_t i = 0; i < m_interfaces.size(); i++) {
            m_previousStats[i].bytesReceived = m_interfaces[i].bytesReceived;
            m_previousStats[i].bytesSent = m_interfaces[i].bytesSent;
            m_previousStats[i].timestamp = currentTime;
        }
        m_lastUpdateTime = currentTime;
        return;
    }

    // Calculate speeds for each interface
    for (size_t i = 0; i < m_interfaces.size(); i++) {
        // Download speed
        uint64_t receivedDelta = 0;
        if (m_interfaces[i].bytesReceived >= m_previousStats[i].bytesReceived) {
            receivedDelta = m_interfaces[i].bytesReceived - m_previousStats[i].bytesReceived;
        }
        m_interfaces[i].downloadSpeed = receivedDelta / timeDelta;

        // Upload speed
        uint64_t sentDelta = 0;
        if (m_interfaces[i].bytesSent >= m_previousStats[i].bytesSent) {
            sentDelta = m_interfaces[i].bytesSent - m_previousStats[i].bytesSent;
        }
        m_interfaces[i].uploadSpeed = sentDelta / timeDelta;

        // Update previous stats
        m_previousStats[i].bytesReceived = m_interfaces[i].bytesReceived;
        m_previousStats[i].bytesSent = m_interfaces[i].bytesSent;
        m_previousStats[i].timestamp = currentTime;
    }

    m_lastUpdateTime = currentTime;
}
