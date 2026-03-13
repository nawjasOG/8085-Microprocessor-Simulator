/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

/* standard c++ includes */
#include <string>

namespace utils {

/**
 * @brief normalize_string(std::string str)
 * @description remove leading, trailing and collapse spaces
 * @param std::string str
 * @return clean string after normalizing
 */
std::string normalize_string(std::string str);

/**
 * @brief get_first_word(const std::string& str)
 * @description gets the first word in a string (space delimited)
 * @param std::string str
 * @return first word as std::string
 */
std::string get_first_word(const std::string& str);

/**
 * @brief to_hex(size_t value, size_t hex_digits)
 * @description convert integert to hex of fixed size
 * @param value(size_t) and hex_digits(size_t)
 * @return hex string with "0x" prefix
 */
std::string to_hex(size_t value, size_t hex_digits);

}  // namespace utils

#endif  // __UTILS_HPP__
