# System Monitor - C++ & Qt6

> Advanced System Monitor built with C++ and Qt6 - Real-time CPU, RAM, Disk, and Network monitoring with beautiful charts

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

## ✨ Features

- 📊 **Real-time CPU Monitoring** - Track CPU usage per core with live charts
- 💾 **RAM Usage Tracking** - Monitor physical and virtual memory consumption
- 💿 **Disk Monitoring** - View disk space, read/write speeds for all drives
- 🌐 **Network Traffic** - Monitor upload/download speeds in real-time
- 📈 **Beautiful Charts** - Powered by Qt Charts for smooth visualizations
- 🎨 **Modern UI** - Clean and intuitive interface with system tray integration

## 🚀 Getting Started

### Prerequisites

- **Visual Studio 2022** (with C++ Desktop Development)
- **Qt 6.5+** - [Download Qt](https://www.qt.io/download)
- **CMake 3.20+**
- **Git**

### Installation

1. Clone the repository:
```bash
git clone https://github.com/KmaNghia18/KmaNghia18-cpp-system-monitor.git
cd cpp-system-monitor
```

2. Configure with CMake:
```bash
cmake -B build -G "Visual Studio 17 2022"
```

3. Build the project:
```bash
cmake --build build --config Release
```

4. Run the application:
```bash
.\build\bin\Release\SystemMonitor.exe
```

## 🏗️ Project Structure

```
cpp-system-monitor/
├── src/
│   ├── monitors/       # System monitoring backends
│   ├── ui/             # Qt UI components
│   ├── utils/          # Helper utilities
│   └── main.cpp        # Application entry point
├── include/
│   ├── monitors/       # Monitor headers
│   ├── ui/             # UI headers
│   └── utils/          # Utility headers
├── resources/          # Icons, styles, assets
└── CMakeLists.txt      # CMake configuration
```

## 🛠️ Technology Stack

- **Language**: C++17
- **UI Framework**: Qt6 (Widgets + Charts)
- **Build System**: CMake
- **APIs**: Windows Performance Counters, IP Helper API

## 📝 Development Progress

Track development progress in the [Projects](https://github.com/KmaNghia18/KmaNghia18-cpp-system-monitor/projects) tab.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👨‍💻 Author

**KmaNghia18**

---

⭐ Star this repository if you find it helpful!
