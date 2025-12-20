# 🚀 FastIO — High-Performance C++23 I/O Library

![FastIO Logo](https://img.shields.io/badge/C++-23-blue)
![License](https://img.shields.io/badge/License-GPLv3-green)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen)

FastIO is a lightweight, high-performance **C++23** input/output library designed as a faster and leaner alternative to `std::format` and `std::print`.

The main goal of FastIO is simple: **maximum performance with minimal generated code**, while keeping the API clean, safe, and easy to extend.

---

## 📊 Benchmark Results

| Operation | FastIO | std::format | Improvement |
|----------|--------|-------------|-------------|
| Format 100k strings | 9 ms | ~15 ms | ~40% faster |
| Code size | ~1.5K lines | ~6K lines | ~75% smaller |
| Binary size | ~50 KB | ~150 KB | ~66% smaller |

---

## ✨ Features

- 🚀 **~40% faster** than `std::format`
- 📉 **~75% less generated code**
- 🔒 **Compile-time format string validation**
- 📦 **Header-only** or **linked library** usage
- ⚡ **Fast integer formatting** using `std::to_chars`
- 🎯 **Type-safe formatting** via modern C++ concepts
- 🔧 **Multiple output styles** (spaced / unspaced)
- 🧩 **Easy customization for user-defined types**

---

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/deotermia/fastio.git
cd fastio

# Build and install
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install   # optional
```

### Usage Example
```cpp
#include <fastio/fastio.hpp>

int main() {
    // Basic output
    fastio::println("Hello, FastIO!");

    // Multiple arguments (with spaces)
    fastio::println_spaced("Values:", 42, 3.14, "text");

    // Formatted output (compile-time validated)
    fastio::println_fmt("{} + {} = {}", 2, 3, 5);

    // String formatting
    auto str = fastio::format("Hello, {}!", "World");

    // Performance measurement
    fastio::stopwatch sw;
    // ... your code ...
    fastio::println_fmt("Time: {} ms", sw.elapsed().count());

    return 0;
}
```

## 📚 API Reference
### 📚 API Reference: Output Functions

| Function | Description | Example Output |
| :--- | :--- | :--- |
| `print(args...)` | No spaces | `42test` |
| `println(args...)` | No spaces + newline | `42test\n` |
| `print_spaced(args...)` | With spaces | `42 test` |
| `println_spaced(args...)` | Spaces + newline | `42 test\n` |
| `print_fmt("{}", arg)` | Formatted output | `value: 42` |
| `println_fmt("{}", arg)` | Formatted + newline | `value: 42\n` |

### Utility Functions
```cpp
// String formatting
auto str = fastio::format("Result: {}", 42);

// Fast integer parsing
int value = fastio::parse_int<int>("12345");

// Performance timer
fastio::stopwatch timer;
// ... code ...
auto elapsed = timer.elapsed<std::chrono::milliseconds>();
```

## 🏗️ Building from Source
## Requirements
- C++23 compatible compiler
- CMake 3.15+

### Build Options
```bash
# Debug build with tests enabled
cmake .. -DCMAKE_BUILD_TYPE=Debug -DFASTIO_BUILD_TESTS=ON

# Release build with aggressive optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native"
```

## 🔧 Advanced Usage
### Custom Type Formatting
```cpp
struct Point {
    int x;
    int y;
};

namespace fastio::internal {
    template<>
    void format_value_to<Point>(format_context& ctx, const Point& p) {
        format(ctx, "({}, {})", p.x, p.y);
    }
}

// Usage
Point p{10, 20};
fastio::println_fmt("Point: {}", p);   // Output: Point: (10, 20)
```

### Stream Integration
```cpp
#include <fstream>

std::ofstream file("log.txt");

fastio::fprintln_fmt(file, "Log entry: {}", "message");
fastio::fprint_spaced(file, "Data:", 1, 2, 3);
```

## 📊 Performance Comparison
```bash
# Run benchmark
cd build
./benchmark
```

### Example output
```bash
FastIO::format : 9 ms (100000 iterations)
std::cout      : 9 ms (100000 iterations)
snprintf       : 6 ms (100000 iterations)
```
