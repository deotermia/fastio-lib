/* test_basic.cpp - Basic tests for FastIO.
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
#include <cassert>
#include <sstream>
#include <iostream>

int main() {
    std::cout << "Running FastIO tests...\n";
    
    // Test 1: Basic print
    {
        std::stringstream ss;
        fastio::fprintln(ss, "Hello, World!");
        std::string result = ss.str();
        std::cout << "Test 1: '" << result << "'\n";
        assert(result == "Hello, World!\n");
        std::cout << "✓ Test 1 passed\n";
    }
    
    // Test 2: Multiple arguments
    {
        std::stringstream ss;
        fastio::fprint(ss, 42, "test", 3.14);
        std::string result = ss.str();
        std::cout << "Test 2: '" << result << "'\n";
        assert(result == "42test3.14");
        std::cout << "✓ Test 2 passed\n";
    }
    
    // Test 3: Multiple arguments
    {
        std::stringstream ss;
        fastio::fprint_spaced(ss, 42, "test", 3.14);
        std::string result = ss.str();
        std::cout << "Test 3: '" << result << "'\n";
        assert(result == "42 test 3.14");
        std::cout << "✓ Test 3 passed\n";
    }
    
    // Test 4: Format
    {
        auto result = fastio::format("{} + {} = {}", 2, 3, 5);
        std::cout << "Test 4: '" << result << "'\n";
        assert(result == "2 + 3 = 5");
        std::cout << "✓ Test 4 passed\n";
    }
    
    // Test 5: Formatted print
    {
        std::stringstream ss;
        fastio::fprintln_fmt(ss, "{} + {} = {}", 2, 3, 5);
        std::string result = ss.str();
        std::cout << "Test 5: '" << result << "'\n";
        assert(result == "2 + 3 = 5\n");
        std::cout << "✓ Test 5 passed\n";
    }
    
    // Test 6: Boolean
    {
        std::stringstream ss;
        fastio::fprint_spaced(ss, true, false);
        std::string result = ss.str();
        std::cout << "Test 6: '" << result << "'\n";
        assert(result == "true false");
        std::cout << "✓ Test 6 passed\n";
    }
    
    // Test 7: Character
    {
        std::stringstream ss;
        fastio::fprint_spaced(ss, 'A', 'B', 'C');
        std::string result = ss.str();
        std::cout << "Test 7: '" << result << "'\n";
        assert(result == "A B C");
        std::cout << "✓ Test 7 passed\n";
    }
    
    // Test 8: Boolean
    {
        std::stringstream ss;
        fastio::fprint(ss, true, false);
        std::string result = ss.str();
        std::cout << "Test 8: '" << result << "'\n";
        assert(result == "truefalse");
        std::cout << "✓ Test 8 passed\n";
    }
    
    std::cout << "\n✅ All tests passed!\n";
    return 0;
}