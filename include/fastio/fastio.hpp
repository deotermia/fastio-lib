/* fastio.hpp - Main header file for FastIO library.
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

#ifndef FASTIO_HPP
#define FASTIO_HPP

#include "core.hpp"
#include "format.hpp"
#include "print.hpp"
#include "input.hpp"
#include <chrono>
#include <algorithm>

namespace fastio {

#define FASTIO_FMT(str) fastio::compile_format_string(str)

class stopwatch
{
public:
  stopwatch() : start_(std::chrono::high_resolution_clock::now()) {}

  template<typename Duration = std::chrono::milliseconds>
  auto elapsed() const
  {
    return std::chrono::duration_cast<Duration>
      (std::chrono::high_resolution_clock::now() - start_);
  }

  void reset() { start_ = std::chrono::high_resolution_clock::now(); }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

template<typename T>
  requires std::integral<T>
inline T parse_int(std::string_view str)
{
  T value;
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

  if (ec != std::errc())
    {
      throw std::runtime_error("Failed to parse integer");
    }

  return value;
}

template<typename T>
  requires std::integral<T>
inline char* write_int(char* buffer, T value)
{
  if constexpr (std::signed_integral<T>)
    {
      if (value < 0)
        {
          *buffer++ = '-';
          return write_int(buffer, static_cast<std::make_unsigned_t<T>>(-value));
        }
    }

  char* start = buffer;

  do
    {
      *buffer++ = static_cast<char>('0' + (value % 10));
      value /= 10;
    }
  while (value > 0);

  std::reverse(start, buffer);
  return buffer;
}

} // namespace fastio

#endif // FASTIO_HPP