/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <ncurses_facade.hpp>

#include "include/app_controller.hpp"
#include "include/view.hpp"
#include "include/command.hpp"

AppController::AppController(ViewUI& view) : __view(view) {
    __view.initialize();
}

void AppController::run() {
    while (true) {
        int ch = __view.read();
        if (ch == KEY_ENTER_) {
            std::string instruction = __view.editor->get_line();
            __source_code.push_back(instruction);
            std::shared_ptr<ICommand> cmd = ICommand::get_command(instruction);
            __view.update(cmd->get_opcode());
            continue;
        }
        ch = toupper(ch);
        if (!valid_character(ch)) continue;
        __view.update(ch);
        if (ch == 'Q') break;
    }
}

bool AppController::valid_character(int ch) {
    return std::isalnum(ch) || ch == ' ' || ch == ',';
}

void AppController::update_code() {

}
