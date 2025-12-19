/* print.hpp - Print and println functions for FastIO.
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

#ifndef FASTIO_PRINT_HPP
#define FASTIO_PRINT_HPP

#include "format.hpp"
#include <iostream>
#include <charconv>

namespace fastio {

namespace internal
{
  /* Fast integer output to stream.  */
  template<typename T>
    requires (std::integral<T> && !std::is_same_v<T, bool>)
  inline void write_integer (std::ostream& os, T value)
  {
    char buffer[32];
    auto [ptr, ec] = std::to_chars (buffer, buffer + sizeof (buffer), value);
    os.write (buffer, ptr - buffer);
  }

  /* Optimized stream output with type dispatch.  */
  template<typename T>
  void stream_out (std::ostream& os, const T& value)
  {
    if constexpr (std::is_same_v<T, bool>)
      {
        os << (value ? "true" : "false");
      }
    else if constexpr (std::integral<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>)
      {
        write_integer (os, value);
      }
    else
      {
        os << value;
      }
  }
  
  /* No spaces between arguments - fold expression.  */
  template<typename... Args>
  void print_impl (std::ostream& os, const Args&... args)
  {
    (stream_out (os, args), ...);
  }
  
  /* With spaces between arguments - unrolled loop.  */
  template<typename First, typename... Rest>
  void print_with_spaces_impl (std::ostream& os, const First& first, const Rest&... rest)
  {
    stream_out (os, first);
    if constexpr (sizeof...(Rest) > 0)
      {
        os.put (' ');
        print_with_spaces_impl (os, rest...);
      }
  }

  inline void print_with_spaces_impl (std::ostream&) {}
}

/* ==================== PUBLIC INTERFACE ==================== */

/* Print arguments without spaces between them.  */
template<typename... Args>
inline void print (const Args&... args)
{
  internal::print_impl (std::cout, args...);
}

template<typename... Args>
inline void println (const Args&... args)
{
  internal::print_impl (std::cout, args...);
  std::cout.put ('\n');
}

/* Print arguments with spaces between them.  */
template<typename... Args>
inline void print_spaced (const Args&... args)
{
  internal::print_with_spaces_impl (std::cout, args...);
}

template<typename... Args>
inline void println_spaced (const Args&... args)
{
  internal::print_with_spaces_impl (std::cout, args...);
  std::cout.put ('\n');
}

/* Print formatted string.  */
template<typename... Args>
inline void print_fmt (compile_format_string fmt, const Args&... args)
{
  std::cout << format (fmt, args...);
}

template<typename... Args>
inline void println_fmt (compile_format_string fmt, const Args&... args)
{
  std::cout << format (fmt, args...) << '\n';
}

/* Stream-specific versions.  */
template<typename... Args>
inline void fprint (std::ostream& os, const Args&... args)
{
  internal::print_impl (os, args...);
}

template<typename... Args>
inline void fprintln (std::ostream& os, const Args&... args)
{
  internal::print_impl (os, args...);
  os.put ('\n');
}

template<typename... Args>
inline void fprint_spaced (std::ostream& os, const Args&... args)
{
  internal::print_with_spaces_impl (os, args...);
}

template<typename... Args>
inline void fprintln_spaced (std::ostream& os, const Args&... args)
{
  internal::print_with_spaces_impl (os, args...);
  os.put ('\n');
}

template<typename... Args>
inline void fprint_fmt (std::ostream& os, compile_format_string fmt, const Args&... args)
{
  os << format (fmt, args...);
}

template<typename... Args>
inline void fprintln_fmt (std::ostream& os, compile_format_string fmt, const Args&... args)
{
  os << format (fmt, args...) << '\n';
}

} /* namespace fastio */

#endif /* FASTIO_PRINT_HPP */