/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

/* project specific c++ includes */
#include "include/ui_builder.hpp"

std::string to_hex(size_t value, size_t hex_digits) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase
       << std::setfill('0') << std::setw(hex_digits) << value;
    return ss.str();
}

void InterfaceUI::initialize() {
    __window.create_window(__length, __width, __start_y, __start_x);
    __window.draw_box();
}

void InterfaceUI::setDimension(const size_t length, const size_t width) {
    __length = length;
    __width = width;
}

void InterfaceUI::setStartPosition(const size_t start_y, const size_t start_x) {
    __start_y = start_y;
    __start_x = start_x;
}

void FlagsUI::add_ui() {
    initialize();
    __window.horizontal_line(2, 1, __width-2);
    const std::string header = "FLAGS";
    size_t header_start_x = (__width - header.size())/2;
    __window.print(1, header_start_x, header);

    for (size_t flag_index = 1; flag_index < flag_names.size(); ++flag_index) {
        __window.vertical_line(3, flag_index * PER_FLAG_SIZE, __length-4);
    }

    for (size_t flag_index = 0; flag_index < flag_names.size(); ++flag_index) {
        const std::string flag_name = flag_names[flag_index] + ": 0";
        const size_t x_pos = flag_index * PER_FLAG_SIZE + 2;
        __window.print(3, x_pos, flag_name);
    }
}

void RegisterUI::add_ui() {
    initialize();
    const size_t total_rows = 3, row_width = 2;
    for (size_t line_index = 1; line_index < total_rows; ++line_index) {
        __window.horizontal_line(line_index*row_width, 1,
                                        __width-2);
    }
    const std::string header = "REGISTERS";
    size_t header_start_x = (__width - header.size())/2;
    __window.print(1, header_start_x, header);

    const size_t wrap_size = __width - 1;
    for (size_t reg_index = 0; reg_index < register_names.size(); ++reg_index) {
        size_t row_index = (reg_index > 3)? 5 : 3;
        size_t x_pos_line = ((reg_index + 1) * PER_REG_SIZE) % wrap_size;
        size_t x_pos = (reg_index * PER_REG_SIZE + 2) % wrap_size;
        __window.vertical_line(3, x_pos_line, __length-4);
        const std::string register_name = register_names[reg_index] + ": 0x00";
        __window.print(row_index, x_pos, register_name);
    }
}

void RegisterUI::refresh(const std::string& reg, const uint8_t value) {
    size_t index = std::find(register_names.begin(), register_names.end(),
                             reg) - register_names.begin();
    size_t y_pos = (index > 3)? 5 : 3;
    size_t x_pos = (index * PER_REG_SIZE + 2) % (__width - 1);
    __window.print(y_pos, x_pos+3, to_hex(value, 2));
}

UIBuilder::UIBuilder(std::unique_ptr<InterfaceUI> ui) : __ui(std::move(ui)) {}

UIBuilder UIBuilder::create(const UIType& ui_type) {
    switch (ui_type) {
        case UIType::Flags:
            return UIBuilder(std::make_unique<FlagsUI>());
        case UIType::Registers:
            return UIBuilder(std::make_unique<RegisterUI>());
    }
    throw std::invalid_argument("unknown ui class");
}

UIBuilder& UIBuilder::setDimension(const size_t length, const size_t width) {
    __ui->setDimension(length, width);
    return *this;
}

UIBuilder& UIBuilder::setStartPosition(const size_t start_y,
                                       const size_t start_x) {
    __ui->setStartPosition(start_y, start_x);
    return *this;
}

/* std::unique_ptr<InterfaceUI> UIBuilder::build() {
    __ui->add_ui();
    return std::move(__ui);
} */
