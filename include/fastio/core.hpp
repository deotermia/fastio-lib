/* core.hpp - Core utilities and type traits for FastIO library.
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

#ifndef FASTIO_CORE_HPP
#define FASTIO_CORE_HPP

#include <type_traits>
#include <concepts>
#include <string_view>
#include <string>
#include <memory>

namespace fastio {

/* Forward declarations for format context.  */
class format_context;

/* Base class for format arguments.  */
class format_arg_base
{
public:
  virtual ~format_arg_base () = default;
  virtual void format (format_context& ctx) const = 0;
  virtual std::size_t estimated_size () const = 0;
};

namespace internal
{
  template<typename T>
  void format_value_to (format_context& ctx, const T& value);
}

/* Compile-time format string with validation.  */
class compile_format_string
{
public:
  template<std::size_t N>
  consteval compile_format_string (const char (&str)[N])
    : data_ (str), size_ (N - 1)
  {
    validate ();
  }

  const char* data () const noexcept { return data_; }
  std::size_t size () const noexcept { return size_; }

private:
  constexpr void validate () const;

  const char* data_;
  std::size_t size_;
};

/* Type traits for efficient integer formatting.  */
template<typename T>
struct is_integral_like : std::false_type {};

/* Specialization for integral types (excluding bool and char).  */
template<typename T>
  requires (std::integral<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>)
struct is_integral_like<T> : std::true_type {};

/* Convenience variable template.  */
template<typename T>
inline constexpr bool is_integral_like_v = is_integral_like<T>::value;

/* Concept for types that can be formatted.  */
template<typename T>
concept formattable = std::convertible_to<T, std::string_view> ||
                     requires (const T& val) {
                       { std::to_string (val) } -> std::convertible_to<std::string>;
                     };

constexpr void compile_format_string::validate () const
{
  int braces = 0;
  
  for (std::size_t i = 0; i < size_; ++i)
    {
      if (data_[i] == '{')
        {
          if (i + 1 < size_ && data_[i + 1] == '{')
            {
              ++i;
              continue;
            }
          ++braces;
        }
      else if (data_[i] == '}')
        {
          if (i + 1 < size_ && data_[i + 1] == '}')
            {
              ++i;
              continue;
            }
          --braces;
          if (braces < 0)
            {
              throw "Unmatched '}' in format string";
            }
        }
    }
  
  if (braces > 0)
    {
      throw "Unmatched '{' in format string";
    }
}

} // namespace fastio

#endif // FASTIO_CORE_HPP