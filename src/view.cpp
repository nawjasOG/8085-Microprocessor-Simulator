/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <ncurses.h>
#include <string>
#include <memory>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/view.hpp"
#include "include/ui_builder.hpp"

// =============================================================================
//                       ViewUI Impl
// =============================================================================
void ViewUI::initialize() {
    add_table();
    add_buttons();
    add_registers();
    add_flags();
    reset_cursor();
}

void ViewUI::add_table() {
    __address_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, ADDRESS_COL_SIZE)
        .setStartPosition(0, 0)
        .setHeader("ADDRESS")
        .build<TableUI>();

    editor = UIBuilder::create(UIType::Editor)
        .setDimension(TABLE_LENGTH, SRC_CODE_SIZE)
        .setStartPosition(0, ADDRESS_COL_SIZE-1)
        .setHeader("SOURCE CODE")
        .build<EditorUI>();

    __machine_code_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, MACHINE_CODE_SIZE)
        .setStartPosition(0, ADDRESS_COL_SIZE + SRC_CODE_SIZE -2)
        .setHeader("MACHINE CODE")
        .build<TableUI>();

    // fixing address window border for rendering proper table
    __address_ui->print(2, 0, ACS_LTEE);
    __address_ui->print(2, ADDRESS_COL_SIZE-1, ACS_PLUS);
    __address_ui->print(0, ADDRESS_COL_SIZE-1, ACS_TTEE);
    __address_ui->print(TABLE_LENGTH-1, ADDRESS_COL_SIZE-1, ACS_BTEE);
    // fixing machine code window border for rendering proper table
    __machine_code_ui->print(2, 0, ACS_PLUS);
    __machine_code_ui->print(2, MACHINE_CODE_SIZE-1, ACS_RTEE);
    __machine_code_ui->print(0, 0, ACS_TTEE);
    __machine_code_ui->print(TABLE_LENGTH-1, 0, ACS_BTEE);
}

void ViewUI::add_buttons() {
    __run_btn = UIBuilder::create(UIType::Button)
        .setDimension(BTN_LENGTH, RUN_BTN_SIZE)
        .setStartPosition(TABLE_LENGTH, 0)
        .setHeader("RUN")
        .build<ButtonUI>();

    __inspect_memory_btn = UIBuilder::create(UIType::Button)
        .setDimension(BTN_LENGTH, INSPECT_BTN_SIZE)
        .setStartPosition(TABLE_LENGTH, RUN_BTN_SIZE-1)
        .setHeader("INSPECT MEMORY")
        .build<ButtonUI>();

    __quit_btn = UIBuilder::create(UIType::Button)
        .setDimension(BTN_LENGTH, QUIT_BTN_SIZE)
        .setStartPosition(TABLE_LENGTH, RUN_BTN_SIZE + INSPECT_BTN_SIZE - 2)
        .setHeader("QUIT")
        .build<ButtonUI>();
}

void ViewUI::add_registers() {
    __register_ui = UIBuilder::create(UIType::Registers)
        .setDimension(REG_WIN_LENGTH, REG_WIN_WIDTH)
        .setStartPosition(0, TABLE_WIDTH)
        .setHeader("REGISTERS")
        .build<RegistersUI>();
}

void ViewUI::add_flags() {
    __flag_ui = UIBuilder::create(UIType::Flags)
        .setDimension(FLAG_WIN_LENGTH, FLAG_WIN_WIDTH)
        .setStartPosition(REG_WIN_LENGTH, TABLE_WIDTH)
        .setHeader("FLAGS")
        .build<FlagsUI>();
}

ButtonType ViewUI::button_clicked() const {
    MouseEvent event = CursesWindow::get_mouse_event();
    if (event.left_click) {
        if (__run_btn->is_click_within_bounds(event.y, event.x)) {
            return ButtonType::RUN_BTN;
        } else if (__inspect_memory_btn->is_click_within_bounds(
                event.y, event.x)) {
            return ButtonType::INSPECT_MEMORY_BTN;
        } else if (__quit_btn->is_click_within_bounds(event.y, event.x)) {
            return ButtonType::QUIT_BTN;
        }
    }
    return ButtonType::NO_BTN;
}

void ViewUI::render_memory_view(const MemoryState& state) {
    const size_t current_line = editor->get_line_number();
    size_t current_pos = 2;
    if (state.machine_code[0] == 0x10) {
        __machine_code_ui->set_attribute(COLOR_PAIR(3));
        __machine_code_ui->print(current_line, current_pos, "SYNTAX ERROR");
        __machine_code_ui->remove_attribute(COLOR_PAIR(3));
        editor->move_to_next_line();
        return;
    }
    for (auto& machine_code : state.machine_code) {
        const std::string code = utils::to_hex(machine_code, 2);
        __machine_code_ui->print(current_line, current_pos, code);
        current_pos += code.size() + 1;
    }
    __address_ui->print(current_line, 2, utils::to_hex(state.address, 4));
    editor->move_to_next_line();
}

void ViewUI::render_cpu_view(const CpuState& state) {
    __register_ui->refresh(state.registers);
    __flag_ui->refresh(state.flags);
}

void ViewUI::reset_cursor() {
    editor->move(__cursor_y, __cursor_x);
}

void ViewUI::save_cursor() {
    __cursor_y = editor->get_line_number();
    __cursor_x = editor->get_column_number();
}
