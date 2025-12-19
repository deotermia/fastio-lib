/* format.cpp - Implementation of formatting functions.
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

#include "fastio/format.hpp"

namespace fastio {
namespace internal {

/* Explicit instantiations for common types.  */
template void format_value_to<int> (format_context&, const int&);
template void format_value_to<long> (format_context&, const long&);
template void format_value_to<long long> (format_context&, const long long&);
template void format_value_to<unsigned> (format_context&, const unsigned&);
template void format_value_to<unsigned long> (format_context&, const unsigned long&);
template void format_value_to<unsigned long long> (format_context&, const unsigned long long&);
template void format_value_to<float> (format_context&, const float&);
template void format_value_to<double> (format_context&, const double&);
template void format_value_to<const char*> (format_context&, const char* const&);
template void format_value_to<std::string> (format_context&, const std::string&);
template void format_value_to<std::string_view> (format_context&, const std::string_view&);
template void format_value_to<bool> (format_context&, const bool&);
template void format_value_to<char> (format_context&, const char&);

/* Explicit instantiation for vformat_to with common sizes.  */
template void vformat_to<1> (format_context&, std::string_view, const format_args<1>&);
template void vformat_to<2> (format_context&, std::string_view, const format_args<2>&);
template void vformat_to<4> (format_context&, std::string_view, const format_args<4>&);
template void vformat_to<8> (format_context&, std::string_view, const format_args<8>&);
template void vformat_to<16> (format_context&, std::string_view, const format_args<16>&);

} /* namespace internal */
} /* namespace fastio */