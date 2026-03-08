/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <iomanip>
#include <sstream>
#include <string>

/* project specific c++ includes */
#include "include/utils.hpp"

namespace utils {

std::string get_first_word(const std::string& str) {
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    return word;
}

std::string to_hex(size_t value, size_t hex_digits) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase
       << std::setfill('0') << std::setw(hex_digits) << value;
    return ss.str();
}

}  // namespace utils
