#include "monitors/NetworkMonitor.h"
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

    // Get the table of network interfaces
    ULONG bufferSize = 0;
    DWORD result = GetIfTable2(nullptr);
    
    MIB_IF_TABLE2* ifTable = nullptr;
    result = GetIfTable2(&ifTable);
    
    if (result != NO_ERROR) {
        std::cerr << "Failed to get network interface table. Error: " << result << std::endl;
        return;
    }

    // Iterate through all interfaces
    for (ULONG i = 0; i < ifTable->NumEntries; i++) {
        MIB_IF_ROW2& row = ifTable->Table[i];
        
        // Filter out loopback and inactive interfaces
        if (row.Type == IF_TYPE_SOFTWARE_LOOPBACK) {
            continue;
        }

        NetworkInterfaceInfo info;
        info.name = row.Alias;
        info.description = row.Description;
        info.bytesReceived = row.InOctets;
        info.bytesSent = row.OutOctets;
        info.downloadSpeed = 0.0;
        info.uploadSpeed = 0.0;
        info.isActive = (row.OperStatus == IfOperStatusUp);
        
        m_interfaces.push_back(info);
    }

    FreeMibTable(ifTable);
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
