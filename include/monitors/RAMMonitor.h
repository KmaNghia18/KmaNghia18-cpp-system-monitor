#ifndef RAMMONITOR_H
#define RAMMONITOR_H

#include <windows.h>
#include <cstdint>

/**
 * @class RAMMonitor
 * @brief Monitors system memory usage using Windows API
 * 
 * This class provides real-time memory information including:
 * - Total physical memory
 * - Available physical memory
 * - Memory usage percentage
 * - Virtual memory statistics
 */
class RAMMonitor {
public:
    /**
     * @brief Constructor
     */
    RAMMonitor();
    
    /**
     * @brief Destructor
     */
    ~RAMMonitor();

    /**
     * @brief Initialize the RAM monitor
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Update memory usage values
     */
    void update();

    /**
     * @brief Get total physical RAM in bytes
     * @return Total RAM in bytes
     */
    uint64_t getTotalPhysicalMemory() const;

    /**
     * @brief Get available physical RAM in bytes
     * @return Available RAM in bytes
     */
    uint64_t getAvailablePhysicalMemory() const;

    /**
     * @brief Get used physical RAM in bytes
     * @return Used RAM in bytes
     */
    uint64_t getUsedPhysicalMemory() const;

    /**
     * @brief Get memory usage percentage
     * @return Memory usage as percentage (0.0 - 100.0)
     */
    double getMemoryUsagePercent() const;

    /**
     * @brief Get total virtual memory in bytes
     * @return Total virtual memory in bytes
     */
    uint64_t getTotalVirtualMemory() const;

    /**
     * @brief Get available virtual memory in bytes
     * @return Available virtual memory in bytes
     */
    uint64_t getAvailableVirtualMemory() const;

    /**
     * @brief Get used virtual memory in bytes
     * @return Used virtual memory in bytes
     */
    uint64_t getUsedVirtualMemory() const;

    /**
     * @brief Check if monitor is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    MEMORYSTATUSEX m_memInfo;  // Windows memory status structure
    bool m_initialized;         // Initialization status

    /**
     * @brief Collect memory data from Windows API
     */
    void collectData();
};

#endif // RAMMONITOR_H
