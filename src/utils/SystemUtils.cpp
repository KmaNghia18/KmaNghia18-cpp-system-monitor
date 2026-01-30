#include "utils/SystemUtils.h"
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>

std::wstring SystemUtils::formatBytes(uint64_t bytes, int precision) {
    const wchar_t* units[] = {L"B", L"KB", L"MB", L"GB", L"TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::wostringstream oss;
    oss << std::fixed << std::setprecision(precision) << size << L" " << units[unitIndex];
    return oss.str();
}

std::wstring SystemUtils::formatSpeed(double bytesPerSec, int precision) {
    const wchar_t* units[] = {L"B/s", L"KB/s", L"MB/s", L"GB/s"};
    int unitIndex = 0;
    double speed = bytesPerSec;

    while (speed >= 1024.0 && unitIndex < 3) {
        speed /= 1024.0;
        unitIndex++;
    }

    std::wostringstream oss;
    oss << std::fixed << std::setprecision(precision) << speed << L" " << units[unitIndex];
    return oss.str();
}

std::wstring SystemUtils::formatPercent(double percent, int precision) {
    std::wostringstream oss;
    oss << std::fixed << std::setprecision(precision) << percent << L"%";
    return oss.str();
}

std::string SystemUtils::wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

std::wstring SystemUtils::stringToWstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}
