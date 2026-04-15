/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cassert>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

/* third-party c++ includes */
#include <ncurses_facade.hpp>

/* project specific c++ includes */
#include "app_controller.hpp"
#include "code_graph.hpp"
#include "ui_builder.hpp"
#include "view.hpp"
#include "model.hpp"
#include "instructions/command.hpp"

// =============================================================================
//                       AppController Impl
// =============================================================================
AppController::AppController(ViewUI& view, Model& model) : __view(view),
        __model(model) {
    __view.initialize();
}

void AppController::run() {
    while (true) {
        int ch = __view.editor->read();
        switch (handle_special_keys(ch)) {
            case AppState::Normal:
                break;
            case AppState::Quit:
                return;
        }
        if (!valid_character(ch)) continue;
        ch = toupper(ch);
        if (ch == 'X') {  // HACK:
            std::string line = __view.editor->get_line();
            if (line.size() >= 2 && line[line.size()-2] == '0') {
                ch = tolower(ch);
            }
        }
        __view.editor->update(ch);
        updateInstruction();
    }
}

void AppController::updateInstruction() {
    size_t index = __view.editor->get_line_number() - EditorUI::START_Y;
    std::string instruction = __view.editor->get_line();
    if (instruction.empty()) {
        __source_code.erase(__source_code.begin()+index);
        __view.clear_memory_view();
        return;
    }
    std::shared_ptr<ICommand> cmd = ICommand::get_command(instruction);
    cmd->set_address(next_address());
    CodeLine codeline = {
        .instruction = instruction,
        .cmd = cmd,
    };
    if (index < __source_code.size()) {
        __source_code[index] = codeline;
    } else {
        __source_code.push_back(codeline);
    }
    notify_memory_state();
}

AppState AppController::handle_special_keys(int ch) {
    switch (ch) {
        // NOTE: this is KEY_ENTER_ and not KEY_ENTER
        case KEY_ENTER_:
            handle_enter();
            break;
        case KEY_BACKSPACE:
            handle_backspace();
            break;
        case KEY_MOUSE:
            return handle_click();
    }
    return AppState::Normal;
}

void AppController::handle_enter() {
    const std::string instruction = __view.editor->get_line();
    // don't move to next line if there is no instruction on current line
    if (!instruction.empty()) {
        __view.editor->move_to_next_line();
    }
}

void AppController::handle_backspace() {
    size_t current_row = __view.editor->get_line_number();
    size_t current_col = __view.editor->get_column_number();
    if (current_col == EditorUI::START_X) {
        size_t index = current_row - EditorUI::START_Y;
        if (index > 0) {
            auto& last = __source_code[index-1];
            __view.editor->move(current_row-1, last.instruction.size()+1);
        }
        return;
    }
    __view.editor->delete_last_char();
    updateInstruction();
}

AppState AppController::handle_click() {
    ButtonType btn = __view.button_clicked();
    switch (btn) {
        case ButtonType::RUN_BTN:
            run_program();
            break;
        case ButtonType::INSPECT_MEMORY_BTN:
            __view.editor->print("inspected");
            break;
        case ButtonType::QUIT_BTN:
            return AppState::Quit;
            break;
        case ButtonType::NO_BTN:
            break;
    }
    return AppState::Normal;
}

uint16_t AppController::next_address() const {
    size_t index = __view.editor->get_line_number() - EditorUI::START_Y;
    if (index == 0) return 0;
    assert(index-1 < __source_code.size());
    auto& last = __source_code[index-1];
    const uint16_t current_addr = last.cmd->get_address();
    if (last.cmd->get_opcode() == INVALID_INSTR) {
        return current_addr;
    }
    const uint16_t next_addr =
        current_addr + last.cmd->get_machine_code().size();
    return next_addr;
}

void AppController::run_program() {
    __view.save_cursor();
    __view.editor->cursor_mode(CURSOR_INVISIBLE);

    auto run_helper = [&]() {
        CodeGraph code_graph(__source_code, __model);

        // popup alert if the program has errors
        if (!code_graph.is_valid_program()) {
            __view.alert->popup();
            return;
        }

        // no errors: execute the program
        size_t line = EditorUI::START_Y;
        InstrNode_h program_counter = code_graph.get_program_counter();
        while (program_counter) {
            __view.editor->highlight_line(line, 1,
                                __view.editor->get_width()-2, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            program_counter = code_graph.run_next_instr(program_counter);
            notify_cpu_state();
            __view.editor->unhighlight_line(line, 1,
                                __view.editor->get_width()-2);
            ++line;
        }
    };

    run_helper();
    __view.editor->cursor_mode(CURSOR_VISIBLE_NORMAL);
    __view.reset_cursor();
}

void AppController::notify_cpu_state() {
    CpuState state = {
        .registers = __model.registers.get_all_registers(),
        .flags = __model.flags.get_all_flags(),
    };
    __view.render_cpu_view(state);
}

void AppController::notify_memory_state() {
    if (__source_code.empty()) {
        // TODO: handle this case
        return;
    }
    auto& codeline = __source_code.back();
    MemoryState state = {
        .machine_code = codeline.cmd->get_machine_code(),
        .address = codeline.cmd->get_address(),
    };
    __view.render_memory_view(state);
}

bool AppController::valid_character(int ch) const {
    return std::isalnum(ch) || ch == SPACE_ASCII || ch == COMMA_ASCII;
}
