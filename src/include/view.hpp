/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __VIEW_HPP__
#define __VIEW_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <vector>
#include <ncurses_facade.hpp>

/* project specific c++ includes */
#include "ui_builder.hpp"

// =============================================================================
//                       ButtonType Enum class
// =============================================================================
enum class ButtonType {
    NO_BTN,
    RUN_BTN,
    INSPECT_MEMORY_BTN,
};

// =============================================================================
//                       ViewState: data-structure for changes in view
// =============================================================================
struct MemoryState {
    std::vector<uint8_t> machine_code;
    uint16_t address;
};

struct CpuState {
    std::vector<uint8_t> registers;
    std::vector<uint8_t> flags;
};

// =============================================================================
//                       ViewUI Class for View
// =============================================================================
class ViewUI {
 public:
    void initialize();
    void add_table();
    void add_buttons();
    void add_registers();
    void add_flags();
    ButtonType button_clicked() const;
    void render_cpu_view(const CpuState& state);
    void render_memory_view(const MemoryState& state);
    void reset_cursor();
    void save_cursor();

    std::unique_ptr<EditorUI> editor;

 private:
    std::unique_ptr<TableUI> __address_ui, __machine_code_ui;
    std::unique_ptr<RegistersUI> __register_ui;
    std::unique_ptr<FlagsUI> __flag_ui;
    std::unique_ptr<ButtonUI> __run_btn, __inspect_memory_btn;
    size_t __cursor_y = EditorUI::START_Y, __cursor_x = EditorUI::START_X;
};

#endif  // __VIEW_HPP__
