#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <windows.h>
#include <pdh.h>
#include <string>
#include <vector>

/**
 * @class CPUMonitor
 * @brief Monitors CPU usage using Windows Performance Data Helper (PDH) API
 * 
 * This class provides real-time CPU usage information including:
 * - Total CPU usage across all cores
 * - Per-core CPU usage
 * - Number of logical processors
 */
class CPUMonitor {
public:
    /**
     * @brief Constructor - Initializes PDH query and counters
     */
    CPUMonitor();
    
    /**
     * @brief Destructor - Cleans up PDH resources
     */
    ~CPUMonitor();

    /**
     * @brief Initialize the CPU monitor
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Update CPU usage values (should be called periodically)
     */
    void update();

    /**
     * @brief Get total CPU usage across all cores
     * @return CPU usage percentage (0.0 - 100.0)
     */
    double getTotalUsage() const;

    /**
     * @brief Get CPU usage for a specific core
     * @param coreIndex Index of the core (0-based)
     * @return CPU usage percentage for the core (0.0 - 100.0)
     */
    double getCoreUsage(int coreIndex) const;

    /**
     * @brief Get number of logical processors
     * @return Number of CPU cores
     */
    int getCoreCount() const;

    /**
     * @brief Check if monitor is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    PDH_HQUERY m_query;                    // PDH query handle
    PDH_HCOUNTER m_totalCounter;            // Total CPU usage counter
    std::vector<PDH_HCOUNTER> m_coreCounters; // Per-core counters
    
    double m_totalUsage;                    // Current total CPU usage
    std::vector<double> m_coreUsages;       // Current per-core usages
    int m_coreCount;                        // Number of CPU cores
    bool m_initialized;                     // Initialization status

    /**
     * @brief Collect CPU usage data from PDH counters
     */
    void collectData();
};

#endif // CPUMONITOR_H
