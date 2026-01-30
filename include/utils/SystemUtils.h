#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <string>
#include <cstdint>

/**
 * @class SystemUtils
 * @brief Utility functions for the system monitor
 * 
 * Provides helper functions for:
 * - Data formatting (bytes to KB/MB/GB)
 * - Percentage formatting
 * - Time formatting
 */
class SystemUtils {
public:
    /**
     * @brief Format bytes to human-readable string (KB, MB, GB, TB)
     * @param bytes Number of bytes
     * @param precision Number of decimal places
     * @return Formatted string (e.g., "1.23 GB")
     */
    static std::wstring formatBytes(uint64_t bytes, int precision = 2);

    /**
     * @brief Format bytes per second to human-readable speed
     * @param bytesPerSec Bytes per second
     * @param precision Number of decimal places
     * @return Formatted string (e.g., "10.5 MB/s")
     */
    static std::wstring formatSpeed(double bytesPerSec, int precision = 2);

    /**
     * @brief Format percentage value
     * @param percent Percentage value (0.0 - 100.0)
     * @param precision Number of decimal places
     * @return Formatted string (e.g., "65.4%")
     */
    static std::wstring formatPercent(double percent, int precision = 1);

    /**
     * @brief Clamp value between min and max
     * @param value Value to clamp
     * @param min Minimum value
     * @param max Maximum value
     * @return Clamped value
     */
    template<typename T>
    static T clamp(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    /**
     * @brief Convert std::wstring to std::string
     * @param wstr Wide string
     * @return Standard string
     */
    static std::string wstringToString(const std::wstring& wstr);

    /**
     * @brief Convert std::string to std::wstring
     * @param str Standard string
     * @return Wide string
     */
    static std::wstring stringToWstring(const std::string& str);
};

#endif // SYSTEMUTILS_H
