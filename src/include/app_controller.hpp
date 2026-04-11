/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __APP_CONTROLLER_HPP__
#define __APP_CONTROLLER_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "view.hpp"
#include "model.hpp"
#include "command.hpp"

// =============================================================================
//                       AppState enum class
// =============================================================================
enum class AppState {
    Normal,
    Quit,
};

// =============================================================================
//                       AppState enum class
// =============================================================================
struct CodeLine {
    std::string instruction;
    std::shared_ptr<ICommand> cmd;
};

// =============================================================================
//                       App Controller (Presenter in MVP)
// =============================================================================
class AppController {
 public:
    AppController(ViewUI& view, Model& model);

    void run();
    void updateInstruction();
    AppState handle_special_keys(int ch);
    void handle_enter();
    void handle_backspace();
    AppState handle_click();
    uint16_t next_address() const;
    void run_program();
    void notify_cpu_state();
    void notify_memory_state();
    bool valid_character(int ch) const;

 private:
    ViewUI& __view;
    Model& __model;
    std::vector<CodeLine> __source_code;
};

#endif  // __APP_CONTROLLER_HPP__
