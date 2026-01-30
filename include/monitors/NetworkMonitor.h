#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <windows.h>
#include <iphlpapi.h>
#include <string>
#include <vector>
#include <cstdint>

#pragma comment(lib, "iphlpapi.lib")

/**
 * @struct NetworkInterfaceInfo
 * @brief Information about a network interface
 */
struct NetworkInterfaceInfo {
    std::wstring name;              // Interface name
    std::wstring description;       // Interface description
    uint64_t bytesReceived;         // Total bytes received
    uint64_t bytesSent;             // Total bytes sent
    double downloadSpeed;           // Download speed in bytes/sec
    double uploadSpeed;             // Upload speed in bytes/sec
    bool isActive;                  // Is interface currently active
};

/**
 * @class NetworkMonitor
 * @brief Monitors network traffic using Windows IP Helper API
 * 
 * This class provides:
 * - Network interface information
 * - Upload/download speeds
 * - Total bytes transferred
 */
class NetworkMonitor {
public:
    /**
     * @brief Constructor
     */
    NetworkMonitor();
    
    /**
     * @brief Destructor
     */
    ~NetworkMonitor();

    /**
     * @brief Initialize the network monitor
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Update network statistics
     */
    void update();

    /**
     * @brief Get information for all network interfaces
     * @return Vector of NetworkInterfaceInfo structures  
     */
    const std::vector<NetworkInterfaceInfo>& getInterfaceInfo() const;

    /**
     * @brief Get total download speed across all active interfaces (bytes/sec)
     * @return Total download speed
     */
    double getTotalDownloadSpeed() const;

    /**
     * @brief Get total upload speed across all active interfaces (bytes/sec)
     * @return Total upload speed
     */
    double getTotalUploadSpeed() const;

    /**
     * @brief Get total bytes downloaded across all interfaces
     * @return Total bytes downloaded
     */
    uint64_t getTotalBytesDownloaded() const;

    /**
     * @brief Get total bytes uploaded across all interfaces
     * @return Total bytes uploaded
     */
    uint64_t getTotalBytesUploaded() const;

    /**
     * @brief Check if monitor is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    std::vector<NetworkInterfaceInfo> m_interfaces;  // Network interfaces
    
    // Previous values for speed calculation
    struct PreviousStats {
        uint64_t bytesReceived;
        uint64_t bytesSent;
        DWORD timestamp;
    };
    std::vector<PreviousStats> m_previousStats;
    
    bool m_initialized;         // Initialization status
    DWORD m_lastUpdateTime;     // Last update timestamp

    /**
     * @brief Collect network interface data
     */
    void collectData();
    
    /**
     * @brief Calculate speeds based on previous data
     */
    void calculateSpeeds();
};

#endif // NETWORKMONITOR_H
