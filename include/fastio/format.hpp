/* format.hpp - Formatting utilities for FastIO library.
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

#ifndef FASTIO_FORMAT_HPP
#define FASTIO_FORMAT_HPP

#include "core.hpp"
#include <charconv>
#include <sstream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <array>
#include <cstring>

namespace fastio {

/* Optimized format context with pre-allocation.  */
class format_context
{
public:
  /* Constructor with initial capacity.  */
  explicit format_context (std::size_t initial_capacity = 256)
  {
    m_buffer.reserve (initial_capacity);
  }

  /* Append single character.  */
  void push_back (char ch)
  {
    m_buffer.push_back (ch);
  }

  /* Append string view - optimized for small strings.  */
  void append (std::string_view str)
  {
    m_buffer.append (str);
  }

  /* Get resulting string (move semantics).  */
  std::string str () &&
  {
    return std::move (m_buffer);
  }

  std::string str () const &
  {
    return m_buffer;
  }

  /* Clear and reuse buffer.  */
  void clear () noexcept
  {
    m_buffer.clear ();
  }

  /* Get current size.  */
  std::size_t size () const noexcept
  {
    return m_buffer.size ();
  }

private:
  std::string m_buffer;
};

/* Template class for typed format arguments with SBO.  */
template<typename T>
class format_arg : public format_arg_base
{
public:
  explicit format_arg (const T& value) : m_value (value) {}

  void format (format_context& ctx) const override
  {
    internal::format_value_to (ctx, m_value);
  }

  std::size_t estimated_size () const override
  {
    if constexpr (std::is_same_v<T, bool>)
      {
        return m_value ? 4 : 5; /* "true" or "false".  */
      }
    else if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>)
      {
        /* Max digits for integer: log10(2^bits) + sign.  */
        if constexpr (std::is_signed_v<T>)
          {
            return sizeof (T) * 3 + 2; /* Conservative estimate.  */
          }
        else
          {
            return sizeof (T) * 3;
          }
      }
    else if constexpr (std::is_same_v<T, char>)
      {
        return 1;
      }
    else if constexpr (std::is_same_v<T, const char*>)
      {
        return std::strlen (m_value);
      }
    else if constexpr (std::is_convertible_v<T, std::string_view>)
      {
        return std::string_view (m_value).size ();
      }
    else
      {
        return 32; /* Default estimate.  */
      }
  }

private:
  const T& m_value;
};

/* Optimized storage for format arguments - fixed-size array.  */
template<std::size_t MaxArgs = 16>
class format_args
{
public:
  template<typename... Args>
  format_args (const Args&... args)
    : m_count (sizeof...(Args))
  {
    static_assert (sizeof...(Args) <= MaxArgs,
                   "Too many format arguments");
    std::size_t i = 0;
    ((m_args[i++] = std::make_unique<format_arg<Args>> (args)), ...);
  }

  /* Get argument by index.  */
  const format_arg_base* get (std::size_t i) const noexcept
  {
    return i < m_count ? m_args[i].get () : nullptr;
  }

  /* Get number of arguments.  */
  std::size_t size () const noexcept
  {
    return m_count;
  }

  /* Estimate total size needed.  */
  std::size_t estimate_total_size () const
  {
    std::size_t total = 0;
    for (std::size_t i = 0; i < m_count; ++i)
      {
        total += m_args[i]->estimated_size ();
      }
    return total;
  }

private:
  std::array<std::unique_ptr<format_arg_base>, MaxArgs> m_args;
  std::size_t m_count;
};

/* Internal implementation with optimizations.  */
namespace internal
{
  /* Fast integer to string conversion.  */
  template<typename T>
    requires (std::integral<T> && !std::is_same_v<T, bool>)
  inline void format_integer (format_context& ctx, T value)
  {
    char buffer[32];
    auto [ptr, ec] = std::to_chars (buffer, buffer + sizeof (buffer), value);
    ctx.append (std::string_view (buffer, ptr - buffer));
  }

  /* Format single value to context - optimized.  */
  template<typename T>
  void format_value_to (format_context& ctx, const T& value)
  {
    if constexpr (std::is_same_v<T, bool>)
      {
        ctx.append (value ? "true" : "false");
      }
    else if constexpr (std::integral<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>)
      {
        format_integer (ctx, value);
      }
    else if constexpr (std::is_same_v<T, char>)
      {
        ctx.push_back (value);
      }
    else if constexpr (std::is_convertible_v<T, std::string_view>)
      {
        ctx.append (std::string_view (value));
      }
    else
      {
        /* Fallback with small string optimization.  */
        thread_local std::ostringstream oss;
        oss.str ("");
        oss << value;
        ctx.append (oss.str ());
      }
  }

  /* Parse format string and write formatted output - optimized.  */
  template<std::size_t MaxArgs>
  void vformat_to (format_context& ctx,
                   std::string_view fmt,
                   const format_args<MaxArgs>& args)
  {
    std::size_t arg_index = 0;
    const char* pos = fmt.data ();
    const char* end = pos + fmt.size ();
    const char* literal_start = pos;

    while (pos < end)
      {
        if (*pos == '{')
          {
            if (pos + 1 < end && *(pos + 1) == '{')
              {
                /* Append literal part before escaped brace.  */
                if (pos > literal_start)
                  {
                    ctx.append (std::string_view (literal_start,
                                                  pos - literal_start));
                  }
                ctx.push_back ('{');
                pos += 2;
                literal_start = pos;
                continue;
              }

            /* Append literal part before placeholder.  */
            if (pos > literal_start)
              {
                ctx.append (std::string_view (literal_start,
                                              pos - literal_start));
              }

            /* Handle placeholder.  */
            if (arg_index >= args.size ())
              {
                throw std::runtime_error ("Not enough arguments for format string");
              }

            args.get (arg_index)->format (ctx);
            ++arg_index;

            /* Find closing brace.  */
            ++pos;
            while (pos < end && *pos != '}')
              {
                ++pos;
              }
            if (pos == end)
              {
                throw std::runtime_error ("Unclosed placeholder");
              }
            ++pos;
            literal_start = pos;
          }
        else if (*pos == '}')
          {
            if (pos + 1 < end && *(pos + 1) == '}')
              {
                if (pos > literal_start)
                  {
                    ctx.append (std::string_view (literal_start,
                                                  pos - literal_start));
                  }
                ctx.push_back ('}');
                pos += 2;
                literal_start = pos;
                continue;
              }
            throw std::runtime_error ("Unmatched '}'");
          }
        else
          {
            ++pos;
          }
      }

    /* Append remaining literal part.  */
    if (pos > literal_start)
      {
        ctx.append (std::string_view (literal_start, pos - literal_start));
      }
  }
}

/* Main formatting function with capacity estimation.  */
template<typename... Args>
std::string format (compile_format_string fmt, const Args&... args)
{
  format_args<sizeof...(Args)> fargs (args...);
  
  /* Estimate required capacity.  */
  std::size_t estimated_size = fmt.size () + fargs.estimate_total_size ();
  format_context ctx (estimated_size);
  
  internal::vformat_to (ctx, std::string_view (fmt.data (), fmt.size ()), fargs);
  return std::move (ctx).str ();
}

} /* namespace fastio */

#endif /* FASTIO_FORMAT_HPP */