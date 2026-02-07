# 🚀 **CPU Process Scheduler Simulator - Release {VERSION}**

A C++ implementation of various CPU scheduling algorithms including FCFS, SJF and MLFQ Priority.

## 📦 Downloads

**Pre-compiled Tarballs (Linux x64):**
- `process-scheduler-release.tar.gz` - Optimized production build (recommended)
- `process-scheduler-debug.tar.gz` - Debug build with symbols for development

**Documentation:**
- `documentation.zip` - Project documentation (PDFs)

## 🔧 Build Information
- **Compiler:** g++ (Ubuntu 22.04+)
- **Standard:** C++11

## 📝 Usage
- **Linux:**
```bash
# Download and extract
tar -xzf process-scheduler-release.tar.gz
cd process-scheduler-release

# Run the scheduler
./process-scheduler
```

## 🐛 Debug Build

For debugging or development:
```bash
# Download and extract
tar -xzf process-scheduler-debug.tar.gz
cd process-scheduler-debug

# Run the scheduler
gdb ./process-scheduler
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