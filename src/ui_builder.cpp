/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <ncurses.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/* project specific c++ includes */
#include "utils.hpp"
#include "ui_builder.hpp"

// =============================================================================
//                       InterfaceUI Impl
// =============================================================================
void InterfaceUI::initialize(bool border) {
    create_window(__length, __width, __start_y, __start_x);
    border ? draw_box() : (void)0;
    set_echo(false);
    CursesWindow::enable_left_mouse();
}

void InterfaceUI::setDimension(const size_t length, const size_t width) {
    __length = length;
    __width = width;
}

void InterfaceUI::setStartPosition(const size_t start_y, const size_t start_x) {
    __start_y = start_y;
    __start_x = start_x;
}

void InterfaceUI::setHeader(const std::string& header) {
    __header = header;
}

// =============================================================================
//                       TableUI Impl
// =============================================================================
void TableUI::add_ui() {
    initialize();
    horizontal_line(2, 1, __width-2);
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(2));
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(2));
}

// =============================================================================
//                       FlagsUI Impl
// =============================================================================
void FlagsUI::add_ui() {
    initialize();
    horizontal_line(2, 1, __width-2);
    print(2, 0, ACS_LTEE);
    print(2, __width-1, ACS_RTEE);
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(2));
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(2));

    for (size_t flag_index = 1; flag_index < flag_names.size(); ++flag_index) {
        size_t x_pos_line = flag_index * PER_FLAG_SIZE;
        vertical_line(3, x_pos_line, __length-4);
        print(2, x_pos_line, ACS_TTEE);
        print(__length-1, x_pos_line, ACS_BTEE);
    }

    for (size_t flag_index = 0; flag_index < flag_names.size(); ++flag_index) {
        const std::string flag_name =
            std::string(flag_names[flag_index]) + ": 0";
        const size_t x_pos = flag_index * PER_FLAG_SIZE + 2;
        print(3, x_pos, flag_name);
    }
}

void FlagsUI::refresh(const std::vector<uint8_t>& flags) {
    for (size_t index = 0; index < flags.size(); ++index) {
        size_t x_pos = index * PER_FLAG_SIZE + 2;
        size_t size = flag_names[index].size();
        print(3, x_pos+size+2, flags[index] == 0 ? "0" : "1");
    }
}

// =============================================================================
//                       RegisterUI Impl
// =============================================================================
void RegistersUI::add_ui() {
    initialize();
    const size_t total_rows = 3, row_width = 2;
    for (size_t line_index = 1; line_index < total_rows; ++line_index) {
        horizontal_line(line_index*row_width, 1, __width-2);
    }
    print(2, 0, ACS_LTEE);
    print(2, __width-1, ACS_RTEE);
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(2));
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(2));

    const size_t wrap_size = __width - 1;
    for (size_t reg_index = 0; reg_index < register_names.size(); ++reg_index) {
        size_t row_index = (reg_index > 3)? 5 : 3;
        size_t x_pos_line = ((reg_index + 1) * PER_REG_SIZE) % wrap_size;
        size_t x_pos = (reg_index * PER_REG_SIZE + 2) % wrap_size;
        vertical_line(3, x_pos_line, __length-4);
        x_pos_line > 0 ? print(2, x_pos_line, ACS_TTEE) : (void)0;
        x_pos_line > 0 ? print(__length-1, x_pos_line, ACS_BTEE) : (void)0;
        const std::string register_name =
            std::string(register_names[reg_index]) + ": 0x00";
        print(row_index, x_pos, register_name);
    }
    for (size_t index = 0; index < 5; ++index) {
        size_t x_pos_line = index * PER_REG_SIZE;
        if (index == 0) print(4, x_pos_line, ACS_LTEE);
        else if (index == 4) print(4, x_pos_line, ACS_RTEE);
        else print(4, x_pos_line, ACS_PLUS);
    }
}

void RegistersUI::refresh(const std::vector<uint8_t>& registers) {
    for (size_t index = 0; index < registers.size(); ++index) {
        size_t y_pos = (index > 3)? 5 : 3;
        size_t x_pos = (index * PER_REG_SIZE + 2) % (__width - 1);
        print(y_pos, x_pos+3, utils::to_hex(registers[index], 2));
    }
}

// =============================================================================
//                       SpecialRegistersUI Impl
// =============================================================================
void SpecialRegistersUI::add_ui() {
    initialize();
    horizontal_line(2, 1, __width-2);
    print(2, 0, ACS_LTEE);
    print(2, __width-1, ACS_RTEE);
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(2));
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(2));

    for (size_t reg_index = 1; reg_index < register_names.size(); ++reg_index) {
        size_t x_pos_line = __width/2;
        vertical_line(3, x_pos_line, __length-4);
        print(2, x_pos_line, ACS_TTEE);
        print(__length-1, x_pos_line, ACS_BTEE);
    }

    for (size_t reg_index = 0; reg_index < register_names.size(); ++reg_index) {
        const std::string register_name =
            std::string(register_names[reg_index]) + ": 0x0000";
        const size_t x_pos = reg_index * (__width/2) + (__width/2 - 10)/2;
        print(3, x_pos, register_name);
    }
}

void SpecialRegistersUI::refresh(const uint16_t pc, const uint16_t sp) {
    // refresh program counter
    size_t x_pos = (__width/2 - 10)/2;
    size_t size = register_names[0].size();
    print(3, x_pos+size+2, utils::to_hex(pc, 4));

    // refresh stack pointer
    x_pos = __width/2 + (__width/2 - 10)/2;
    size = register_names[1].size();
    print(3, x_pos+size+2, utils::to_hex(sp, 4));
}

// =============================================================================
//                       ButtonUI Impl
// =============================================================================
void ButtonUI::add_ui() {
    initialize(false);
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(2)|A_STANDOUT);
    for (size_t row = 0; row < __length; ++row) {
        for (size_t col = 1; col < __width-1; ++col) {
            print(row, col, 0x20);
        }
    }
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(2)|A_STANDOUT);
}

// =============================================================================
//                       AlertUI Impl
// =============================================================================
void AlertUI::add_ui() {
    initialize();
    size_t header_start_x = (__width - __header.size())/2;
    set_attribute(COLOR_PAIR(3));
    print(1, header_start_x, __header);
    remove_attribute(COLOR_PAIR(3));
    std::string key_press = "press any key to close";
    header_start_x = (__width - key_press.size())/2;
    print(2, header_start_x, key_press);
    hide();
}

void AlertUI::popup() const {
    show();
    read();
    hide();
}

// =============================================================================
//                       Editor Impl
// =============================================================================
void EditorUI::update(int ch) {
    print(ch);
}

size_t EditorUI::get_line_number() const {
    return get_cursor_y();
}

size_t EditorUI::get_column_number() const {
    return get_cursor_x();
}

void EditorUI::move_to_next_line() {
    move(get_line_number()+1, START_X);
}

void EditorUI::delete_last_char() {
    print(get_line_number(), get_column_number()-1, ' ');
    movex(get_column_number()-1);
}

std::string EditorUI::get_line() const {
    size_t current_y = get_line_number();
    size_t current_x = get_column_number();
    std::string instruction{};
    bool is_empty = true;
    for (size_t x = START_X; x <= current_x; ++x) {
        instruction.append(1, get_char_at(current_y, x));
        if (SPACE_ASCII != instruction.back()) {
            is_empty = false;
        }
    }
    return is_empty? std::string() : instruction;
}

// =============================================================================
//                       UIBuilder Impl
// =============================================================================
UIBuilder::UIBuilder(std::unique_ptr<InterfaceUI> ui) : __ui(std::move(ui)) {}

UIBuilder UIBuilder::create(const UIType& ui_type) {
    switch (ui_type) {
        case UIType::Alert:
            return UIBuilder(std::make_unique<AlertUI>());
        case UIType::Button:
            return UIBuilder(std::make_unique<ButtonUI>());
        case UIType::Flags:
            return UIBuilder(std::make_unique<FlagsUI>());
        case UIType::Registers:
            return UIBuilder(std::make_unique<RegistersUI>());
        case UIType::SpecialRegisters:
            return UIBuilder(std::make_unique<SpecialRegistersUI>());
        case UIType::Table:
            return UIBuilder(std::make_unique<TableUI>());
        case UIType::Editor:
            return UIBuilder(std::make_unique<EditorUI>());
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

UIBuilder& UIBuilder::setHeader(const std::string& header) {
    __ui->setHeader(header);
    return *this;
}

/* std::unique_ptr<InterfaceUI> UIBuilder::build() {
    __ui->add_ui();
    return std::move(__ui);
} */
