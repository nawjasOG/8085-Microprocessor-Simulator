/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cctype>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

/* third-party c++ includes */
#include <ncurses_facade.hpp>

/* project specific c++ includes */
#include "include/app_controller.hpp"
#include "include/view.hpp"
#include "include/model.hpp"
#include "include/command.hpp"

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
    }
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
    std::string instruction = __view.editor->get_line();
    std::shared_ptr<ICommand> cmd = ICommand::get_command(instruction);
    cmd->set_address(next_address());
    __source_code.push_back(cmd);
    notify_memory_state();
}

void AppController::handle_backspace() {
    size_t current_row = __view.editor->get_line_number();
    size_t current_col = __view.editor->get_column_number();
    if (current_col == EditorUI::START_X) {
        if (current_row == EditorUI::START_Y) return;
        // TODO: handle this case
        return;
    }
    __view.editor->delete_last_char();
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
    if (__source_code.empty()) return 0;
    auto last = __source_code.back();
    const uint16_t current_addr = last->get_address();
    const uint16_t next_addr = current_addr + last->get_machine_code().size();
    return next_addr;
}

void AppController::run_program() {
    __view.save_cursor();
    __view.editor->cursor_mode(CURSOR_INVISIBLE);

    auto run_helper = [&]() {
        // first pass of program to scan for errors
        bool has_errors = false;
        for (auto cmd : __source_code) {
            if (cmd->get_opcode() == 0x10) {
                has_errors = true;
                break;
            }
        }
        // popup alert if the program has errors
        if (has_errors) {
            __view.alert->popup();
            return;
        }
        // no errors: execute the program
        size_t line = EditorUI::START_Y;
        for (auto cmd : __source_code) {
            __view.editor->highlight_line(line, 1,
                                __view.editor->get_width()-2, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            if (cmd->execute(__model)) {
                notify_cpu_state();
            }
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
    auto cmd = __source_code.back();
    MemoryState state = {
        .machine_code = cmd->get_machine_code(),
        .address = cmd->get_address(),
    };
    __view.render_memory_view(state);
}

bool AppController::valid_character(int ch) const {
    return std::isalnum(ch) || ch == ' ' || ch == ',';
}
