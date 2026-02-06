# 🚀 **CPU Process Scheduler Simulator - Release {VERSION}**

A C++ implementation of various CPU scheduling algorithms including FCFS, SJF, Priority, and Round Robin.

## 📦 Downloads

**Pre-compiled Binaries (Linux x64):**
- `process-scheduler-release` - Optimized production build (recommended)
- `process-scheduler-debug` - Debug build with symbols for development

**Documentation:**
- `documentation.zip` - Project documentation (PDFs)

## 🔧 Build Information
- **Compiler:** g++ (Ubuntu 22.04+)
- **Standard:** C++11

## 📝 Usage
```bash
# Download and run the Release binary
chmod +x process-scheduler-release
./process-scheduler-release
```

## 🐛 Debug Build

For debugging or development:
```bash
chmod +x process-scheduler-debug
gdb ./process-scheduler-debug
```

## 🛠️ Building from Source
```bash
git clone https://github.com/ADolbyB/process-scheduler.git
cd process-scheduler
```

Compile with debug symbols
```bash
g++ -g -std=c++11 -Wall -Werror *.cpp *.h -o sched_driver
```

Launch with GDB
```bash
gdb ./sched_driver
```

See the full source code and detailed instructions in the repository README.