# Build Instructions cho System Monitor

## Requirements
- Visual Studio 2022 (đã cài)
- Qt 6.5+ with Qt Charts (cài theo hướng dẫn)
- CMake 3.20+ (thường đi kèm VS 2022)

## Build Steps

### 1. Open PowerShell trong project folder
```powershell
cd c:\Users\ADMIN\hacker\cpp-system-monitor
```

### 2. Configure CMake
```powershell
# Nếu Qt được thêm vào PATH tự động:
cmake -B build -G "Visual Studio 17 2022"

# Nếu CMake không tìm thấy Qt, chỉ định path:
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:\Qt\6.6.0\msvc2022_64"
```

### 3. Build project
```powershell
cmake --build build --config Release
```

### 4. Run application
```powershell
.\build\bin\Release\SystemMonitor.exe
```

## Troubleshooting

### Lỗi: "Could not find Qt6"
- Kiểm tra Qt đã cài đúng version (6.x) và compiler (MSVC 2022)
- Thêm `-DCMAKE_PREFIX_PATH` như trên

### Lỗi: "Qt6Charts not found"
- Quay lại Qt Maintenance Tool
- Chọn "Add or remove components"
- Thêm Qt Charts module

### Lỗi khi run: "Missing Qt6Core.dll"
- Copy Qt DLLs vào thư mục exe:
```powershell
copy C:\Qt\6.6.0\msvc2022_64\bin\Qt6*.dll .\build\bin\Release\
```

Hoặc thêm Qt bin vào PATH (đã hướng dẫn ở trên)
