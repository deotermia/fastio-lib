/* example.cpp - Example usage of FastIO.
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

int main() {
    fastio::println("Hello from FastIO!");
    
    fastio::println_spaced("Numbers:", 1, 2, 3, 4, 5);
    
    std::string name = "World";
    int value = 42;
    fastio::println_spaced("Hello,", name, "! The answer is", value);
    
    fastio::println_fmt("Formatted: {} + {} = {}", 2, 3, 5);
    
    fastio::stopwatch sw;
    for (int i = 0; i < 10; ++i) {
        fastio::println_fmt("Iteration {}: {}", i, i * i);
    }
    auto elapsed = sw.elapsed();
    fastio::println_fmt("FastIO format time: {} ms", elapsed.count());
    
    sw.reset();
    for (int i = 0; i < 10; ++i) {
        std::cout << "Iteration " << i << ": " << i * i << "\n";
    }
    auto std_time = sw.elapsed();
    fastio::println_fmt("std::cout time: {} ms", std_time.count());
    
    return 0;
}