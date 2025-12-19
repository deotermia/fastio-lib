/* input.hpp - Input functions for FastIO library.
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

#ifndef FASTIO_INPUT_HPP
#define FASTIO_INPUT_HPP

#include <iostream>
#include <string>

namespace fastio {

/* Internal implementation details.  */
namespace internal
{
  /* Read single value from stream.  */
  template<typename T>
  void
  input_one (std::istream& is, T& value)
  {
    if constexpr (std::is_same_v<T, std::string>)
      {
        /* Special handling for strings: skip whitespace.  */
        is >> std::ws;
        std::getline (is, value, ' ');
      }
    else
      {
        /* Use standard stream input.  */
        is >> value;
      }
  }

  /* Recursive input implementation.  */
  template<typename First, typename... Rest>
  void
  input_impl (std::istream& is, First& first, Rest&... rest)
  {
    input_one (is, first);
    if constexpr (sizeof...(Rest) > 0)
      {
        input_impl (is, rest...);
      }
  }
}

/* Read single value from stdin.  */
template<typename T>
void
input (T& value)
{
  internal::input_one (std::cin, value);
}

/* Read multiple values from stdin.  */
template<typename... Args>
void
input (Args&... args)
{
  internal::input_impl (std::cin, args...);
}

/* Read from arbitrary input stream.  */
template<typename... Args>
void
finput (std::istream& is, Args&... args)
{
  internal::input_impl (is, args...);
}

/* Read line from stdin.  */
inline std::string
readline ()
{
  std::string line;
  std::getline (std::cin, line);
  return line;
}

} /* namespace fastio */

#endif /* FASTIO_INPUT_HPP */