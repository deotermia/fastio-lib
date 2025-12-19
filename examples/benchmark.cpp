/* benchmark.cpp - Benchmark for FastIO vs std I/O.
   Copyright (C) 2025 deotermia
   
   This file is part of FastIO, a fast I/O library for C++.
   
   FastIO is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   FastIO is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with FastIO.  If not, see <https://www.gnu.org/licenses/>.  */

#include "fastio/fastio.hpp"
#include <chrono>
#include <fstream>

void benchmark_fastio_format(int iterations) {
    fastio::stopwatch sw;
    volatile size_t dummy = 0;
    
    for (int i = 0; i < iterations; ++i) {
        auto str = fastio::format("Iteration {}: value = {}", i, i * 2);
        dummy += str.size();
    }
    
    auto time = sw.elapsed();
    fastio::println("FastIO::format: {} ms ({} iterations)", 
                   time.count(), iterations);
}

void benchmark_fastio_println_format(int iterations) {
    fastio::stopwatch sw;
    
    std::ofstream null_stream("/dev/null");
    
    for (int i = 0; i < iterations; ++i) {
        fastio::fprintln(null_stream, "Iteration {}: value = {}", i, i * 2);
    }
    
    auto time = sw.elapsed();
    fastio::println("FastIO::println with format: {} ms ({} iterations)", 
                   time.count(), iterations);
}

void benchmark_fastio_print_plain(int iterations) {
    fastio::stopwatch sw;
    
    std::ofstream null_stream("/dev/null");
    
    for (int i = 0; i < iterations; ++i) {
        fastio::fprintln(null_stream, "Iteration ", i, ": value = ", i * 2);
    }
    
    auto time = sw.elapsed();
    fastio::println("FastIO::println plain: {} ms ({} iterations)", 
                   time.count(), iterations);
}

void benchmark_cout(int iterations) {
    fastio::stopwatch sw;
    
    std::ofstream null_stream("/dev/null");
    auto old_cout = std::cout.rdbuf(null_stream.rdbuf());
    
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Iteration " << i << ": value = " << i * 2 << "\n";
    }
    
    std::cout.rdbuf(old_cout);
    auto time = sw.elapsed();
    fastio::println("std::cout: {} ms ({} iterations)", 
                   time.count(), iterations);
}

void benchmark_sprintf(int iterations) {
    fastio::stopwatch sw;
    volatile size_t dummy = 0;
    char buffer[256];
    
    for (int i = 0; i < iterations; ++i) {
        int len = snprintf(buffer, sizeof(buffer), 
                          "Iteration %d: value = %d", i, i * 2);
        dummy += len;
    }
    
    auto time = sw.elapsed();
    fastio::println("snprintf: {} ms ({} iterations)", 
                   time.count(), iterations);
}

int main() {
    const int warmup = 1000;
    const int iterations = 100000;
    
    fastio::println("=== FastIO Benchmark ===");
    fastio::println("Iterations: {}", iterations);
    fastio::println("");
    
    for (int i = 0; i < warmup; ++i) {
        auto warm = fastio::format("warmup {}", i);
    }
    
    benchmark_fastio_format(iterations);
    benchmark_fastio_println_format(iterations);
    benchmark_fastio_print_plain(iterations);
    benchmark_cout(iterations);
    benchmark_sprintf(iterations);
    
    fastio::println("");
    fastio::println("Benchmark completed!");
    
    return 0;
}