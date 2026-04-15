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
#include <string_view>

// =============================================================================
//                       namespace: utils (common project utilities)
// =============================================================================
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

// =============================================================================
//                       namespace: mpu (microprocessor-unit)
// =============================================================================
namespace mpu {
// 8085 constants
constexpr std::string_view DATA_X = "INT";
constexpr std::string_view NO_OPERAND = "NA";

// 8085 microprocessor special registers
constexpr std::string_view PC = "PC";
constexpr std::string_view SP = "SP";

// 8085 microprocessor registers
constexpr std::string_view ACCUMULATOR = "A";
constexpr std::string_view REG_B = "B";
constexpr std::string_view REG_C = "C";
constexpr std::string_view REG_D = "D";
constexpr std::string_view REG_E = "E";
constexpr std::string_view REG_H = "H";
constexpr std::string_view REG_L = "L";
constexpr std::string_view REG_M = "M";

// 8085 microprocessor flags
constexpr std::string_view SIGN_FLAG = "S";
constexpr std::string_view ZERO_FLAG = "Z";
constexpr std::string_view AUXIL_CARRY_FLAG = "AC";
constexpr std::string_view PARITY_FLAG = "P";
constexpr std::string_view CARRY_FLAG = "CY";
}  // namespace mpu

#endif  // __UTILS_HPP__
