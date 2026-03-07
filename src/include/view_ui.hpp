/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <algorithm>
#include <cctype>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>

#include "ui_builder.hpp"
#include <ncurses_facade.hpp>

#define TABLE_LENGTH 25
#define ADDRESS_COL_SIZE 11
#define SRC_CODE_SIZE 22
#define MACHINE_CODE_SIZE 22
#define TABLE_WIDTH (ADDRESS_COL_SIZE + SRC_CODE_SIZE + MACHINE_CODE_SIZE)

#define BTN_LENGTH 3
#define RUN_BTN_SIZE (ADDRESS_COL_SIZE + SRC_CODE_SIZE - 1)
#define INSPECT_BTN_SIZE (TABLE_WIDTH - RUN_BTN_SIZE - 2)

class Editor {
 public:
    explicit Editor(CursesWindow &editor_win) : __editor_win(editor_win) {}

 private:
    CursesWindow &__editor_win;
};

class ViewUI {
 public:
    void initialize() {
        add_table();
        add_buttons();
        __register_ui = UIBuilder::create(UIType::Registers)
            .setDimension(REG_WIN_LENGTH, REG_WIN_WIDTH)
            .setStartPosition(0, TABLE_WIDTH)
            .build<RegisterUI>();

        editor = std::make_shared<Editor>(__source_code_win);
        __flag_ui = UIBuilder::create(UIType::Flags)
            .setDimension(FLAG_WIN_LENGTH, FLAG_WIN_WIDTH)
            .setStartPosition(REG_WIN_LENGTH, TABLE_WIDTH)
            .build<FlagsUI>();
    }

    void add_table() {
        size_t last_column_x = 0;
        auto create_column = [&](CursesWindow &generic_win, const size_t width,
                                 const std::string &header) {
            generic_win.create_window(TABLE_LENGTH, width, 0, last_column_x);
            generic_win.draw_box();
            generic_win.horizontal_line(2, 1, width-2);
            size_t header_start_x = (width - header.size())/2;
            generic_win.print(1, header_start_x, header);
            last_column_x += width - 1;
        };

        create_column(__address_win, ADDRESS_COL_SIZE, "ADDRESS");
        create_column(__source_code_win, SRC_CODE_SIZE, "SOURCE CODE");
        create_column(__machine_code_win, MACHINE_CODE_SIZE, "MACHINE CODE");
    }

    void add_buttons() {
        size_t last_btn_x = 0;
        auto create_btn = [&](CursesWindow &generic_btn, const size_t width,
                             const std::string &header) {
            generic_btn.create_window(BTN_LENGTH, width, TABLE_LENGTH,
                                      last_btn_x);
            generic_btn.draw_box();
            size_t header_start_x = (width - header.size())/2;
            generic_btn.print(1, header_start_x, header);
            last_btn_x += width;
        };

        create_btn(__run_btn, RUN_BTN_SIZE, "RUN PROGRAM");
        create_btn(__inspect_memory_btn, INSPECT_BTN_SIZE, "INSPECT MEMORY");
    }

    void run() {
        __source_code_win.move(3, 1);
        __source_code_win.set_echo(false);
        while (true) {
            int ch = __source_code_win.read();
            if (ch == KEY_ESC) {
                size_t y_last = __source_code_win.get_cursor_y();
                size_t x_last = __source_code_win.get_cursor_x();
                __register_ui->refresh("A", 254);
                __source_code_win.move(y_last, x_last);
            }
            if (!std::isalnum(ch) && ch != ' ' && ch != ',') continue;
            ch = toupper(ch);
            __source_code_win.print(ch);
            if (ch == 'Q') break;
        }
    }

 public:
    std::shared_ptr<Editor> editor;

 private:
    CursesWindow __address_win, __source_code_win, __machine_code_win;
    CursesWindow __run_btn, __inspect_memory_btn;
    std::unique_ptr<RegisterUI> __register_ui;
    std::unique_ptr<FlagsUI> __flag_ui;
};
