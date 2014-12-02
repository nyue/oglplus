/**
 *  @file eglplus/string.hpp
 *  @brief String-related typedefs
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_STRING_1303291951_HPP
#define EGLPLUS_STRING_1303291951_HPP

#include <eagine/base/string.hpp>

namespace eglplus {

/// String class
/**
 *  @ingroup eglplus_strings
 */
typedef ::eagine::base::basic_string<char> String;

/// String const reference wrapper
/**
 *  @ingroup eglplus_strings
 */
typedef ::eagine::base::basic_string_ref<const char> StrCRef;

} // namespace eglplus

#endif // include guard
