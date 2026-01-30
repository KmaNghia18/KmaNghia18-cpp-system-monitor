cmake_minimum_required(VERSION 3.20)
project(SystemMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/monitors
    ${CMAKE_SOURCE_DIR}/include/ui
    ${CMAKE_SOURCE_DIR}/include/utils
)

# Source files
set(MONITOR_SOURCES
    src/monitors/CPUMonitor.cpp
    src/monitors/RAMMonitor.cpp
    src/monitors/DiskMonitor.cpp
    src/monitors/NetworkMonitor.cpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/CPUWidget.cpp
    src/ui/RAMWidget.cpp
    src/ui/DiskWidget.cpp
    src/ui/NetworkWidget.cpp
)

set(UTIL_SOURCES
    src/utils/SystemUtils.cpp
)

set(HEADERS
    include/monitors/CPUMonitor.h
    include/monitors/RAMMonitor.h
    include/monitors/DiskMonitor.h
    include/monitors/NetworkMonitor.h
    include/ui/MainWindow.h
    include/ui/CPUWidget.h
    include/ui/RAMWidget.h
    include/ui/DiskWidget.h
    include/ui/NetworkWidget.h
    include/utils/SystemUtils.h
)

# Main executable
add_executable(SystemMonitor
    src/main.cpp
    ${MONITOR_SOURCES}
    ${UI_SOURCES}
    ${UTIL_SOURCES}
    ${HEADERS}
)

# Link Qt6 libraries
target_link_libraries(SystemMonitor
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)

# Link Windows libraries
if(WIN32)
    if(MSVC)
        target_link_libraries(SystemMonitor
            pdh
            iphlpapi
            kernel32
        )
    else()
        # MinGW uses different library names
        target_link_libraries(SystemMonitor
            -lpdh
            -liphlpapi
            -lkernel32
        )
    endif()
endif()

# Set output directory
set_target_properties(SystemMonitor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy Qt DLLs to output directory (Windows only)
if(WIN32)
    add_custom_command(TARGET SystemMonitor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Charts>
        $<TARGET_FILE_DIR:SystemMonitor>
    )
endif()
