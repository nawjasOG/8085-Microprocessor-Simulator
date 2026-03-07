/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "include/app_controller.hpp"
#include "include/view.hpp"

AppController::AppController(ViewUI& view) : __view(view) {
    __view.initialize();
}

void AppController::run() {
    while (true) {
        int ch = __view.read();
        ch = toupper(ch);
        if (!valid_character(ch)) continue;
        __view.update(ch);
        if (ch == 'Q') break;
    }
}

bool AppController::valid_character(int ch) {
    return std::isalnum(ch) || ch == ' ' || ch == ',';
}
