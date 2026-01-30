#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include <windows.h>
#include <pdh.h>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @struct DiskInfo
 * @brief Information about a single disk drive
 */
struct DiskInfo {
    std::wstring driveLetter;      // Drive letter (e.g., "C:")
    uint64_t totalSpace;            // Total space in bytes
    uint64_t freeSpace;             // Free space in bytes
    uint64_t usedSpace;             // Used space in bytes
    double usagePercent;            // Usage percentage
    double readSpeed;               // Read speed in bytes/sec
    double writeSpeed;              // Write speed in bytes/sec
    std::wstring volumeName;        // Volume name
    std::wstring fileSystem;        // File system type (NTFS, FAT32, etc.)
};

/**
 * @class DiskMonitor
 * @brief Monitors disk usage and I/O performance
 * 
 * This class provides:
 * - Disk space information for all drives
 * - Read/write speeds using PDH counters
 * - File system information
 */
class DiskMonitor {
public:
    /**
     * @brief Constructor
     */
    DiskMonitor();
    
    /**
     * @brief Destructor
     */
    ~DiskMonitor();

    /**
     * @brief Initialize the disk monitor
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Update disk usage and speed information
     */
    void update();

    /**
     * @brief Get disk information for all drives
     * @return Vector of DiskInfo structures
     */
    const std::vector<DiskInfo>& getDiskInfo() const;

    /**
     * @brief Get information for a specific drive
     * @param driveLetter Drive letter (e.g., "C:")
     * @return Pointer to DiskInfo or nullptr if not found
     */
    const DiskInfo* getDriveInfo(const std::wstring& driveLetter) const;

    /**
     * @brief Get number of monitored drives
     * @return Number of drives
     */
    int getDriveCount() const;

    /**
     * @brief Check if monitor is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    std::vector<DiskInfo> m_disks;      // Information for all disks
    PDH_HQUERY m_query;                 // PDH query handle
    
    struct SpeedCounters {
        PDH_HCOUNTER readCounter;
        PDH_HCOUNTER writeCounter;
        std::wstring diskName;
    };
    std::vector<SpeedCounters> m_speedCounters; // Speed counters for each disk
    
    bool m_initialized;                 // Initialization status

    /**
     * @brief Enumerate all available drives
     */
    void enumerateDrives();

    /**
     * @brief Collect disk space information
     */
    void collectSpaceInfo();

    /**
     * @brief Collect disk speed information from PDH
     */
    void collectSpeedInfo();
    
    /**
     * @brief Setup PDH counters for disk I/O monitoring
     */
    bool setupSpeedCounters();
};

#endif // DISKMONITOR_H
