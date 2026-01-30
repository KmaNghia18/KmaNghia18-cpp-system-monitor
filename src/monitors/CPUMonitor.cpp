#include "monitors/CPUMonitor.h"
#include <iostream>

#pragma comment(lib, "pdh.lib")

CPUMonitor::CPUMonitor()
    : m_query(nullptr)
    , m_totalCounter(nullptr)
    , m_totalUsage(0.0)
    , m_coreCount(0)
    , m_initialized(false)
{
}

CPUMonitor::~CPUMonitor() {
    // Clean up PDH resources
    if (m_totalCounter) {
        PdhRemoveCounter(m_totalCounter);
    }
    
    for (auto& counter : m_coreCounters) {
        if (counter) {
            PdhRemoveCounter(counter);
        }
    }
    
    if (m_query) {
        PdhCloseQuery(m_query);
    }
}

bool CPUMonitor::initialize() {
    if (m_initialized) {
        return true;
    }

    // Get number of processors
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    m_coreCount = sysInfo.dwNumberOfProcessors;

    // Create PDH query
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &m_query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to open PDH query. Error: " << status << std::endl;
        return false;
    }

    // Add total CPU usage counter
    status = PdhAddCounter(m_query, L"\\Processor(_Total)\\% Processor Time", 0, &m_totalCounter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to add total CPU counter. Error: " << status << std::endl;
        PdhCloseQuery(m_query);
        m_query = nullptr;
        return false;
    }

    // Add per-core counters
    m_coreCounters.resize(m_coreCount);
    m_coreUsages.resize(m_coreCount, 0.0);

    for (int i = 0; i < m_coreCount; i++) {
        std::wstring counterPath = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
        status = PdhAddCounter(m_query, counterPath.c_str(), 0, &m_coreCounters[i]);
        
        if (status != ERROR_SUCCESS) {
            std::cerr << "Failed to add CPU core " << i << " counter. Error: " << status << std::endl;
            // Continue even if one core fails
        }
    }

    // Collect initial sample (required for percentage calculation)
    status = PdhCollectQueryData(m_query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect initial PDH data. Error: " << status << std::endl;
        return false;
    }

    m_initialized = true;
    return true;
}

void CPUMonitor::update() {
    if (!m_initialized) {
        return;
    }

    collectData();
}

double CPUMonitor::getTotalUsage() const {
    return m_totalUsage;
}

double CPUMonitor::getCoreUsage(int coreIndex) const {
    if (coreIndex < 0 || coreIndex >= m_coreCount) {
        return 0.0;
    }
    return m_coreUsages[coreIndex];
}

int CPUMonitor::getCoreCount() const {
    return m_coreCount;
}

bool CPUMonitor::isInitialized() const {
    return m_initialized;
}

void CPUMonitor::collectData() {
    // Collect query data
    PDH_STATUS status = PdhCollectQueryData(m_query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect PDH data. Error: " << status << std::endl;
        return;
    }

    // Get total CPU usage
    PDH_FMT_COUNTERVALUE counterValue;
    status = PdhGetFormattedCounterValue(m_totalCounter, PDH_FMT_DOUBLE, nullptr, &counterValue);
    if (status == ERROR_SUCCESS) {
        m_totalUsage = counterValue.doubleValue;
        // Clamp to valid range
        if (m_totalUsage < 0.0) m_totalUsage = 0.0;
        if (m_totalUsage > 100.0) m_totalUsage = 100.0;
    }

    // Get per-core CPU usage
    for (int i = 0; i < m_coreCount; i++) {
        if (m_coreCounters[i]) {
            status = PdhGetFormattedCounterValue(m_coreCounters[i], PDH_FMT_DOUBLE, nullptr, &counterValue);
            if (status == ERROR_SUCCESS) {
                m_coreUsages[i] = counterValue.doubleValue;
                // Clamp to valid range
                if (m_coreUsages[i] < 0.0) m_coreUsages[i] = 0.0;
                if (m_coreUsages[i] > 100.0) m_coreUsages[i] = 100.0;
            }
        }
    }
}
