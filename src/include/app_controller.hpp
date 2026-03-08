/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __APP_CONTROLLER_HPP__
#define __APP_CONTROLLER_HPP__

#include <string>
#include <vector>
#include "view.hpp"

class AppController {
 public:
    AppController(ViewUI& view);

    void run();
    bool valid_character(int ch);
    void update_code();

 private:
    ViewUI& __view;
    std::vector<std::string> __source_code;
};

#endif  // __APP_CONTROLLER_HPP__
