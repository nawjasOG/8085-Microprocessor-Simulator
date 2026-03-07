/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __UI_BUILDER_HPP__
#define __UI_BUILDER_HPP__

/* standard c++ includes */
#include <memory>
#include <string>
#include <vector>

/* third-party c++ includes */
#include <ncurses_facade.hpp>

#define TABLE_LENGTH 25
#define ADDRESS_COL_SIZE 11
#define SRC_CODE_SIZE 22
#define MACHINE_CODE_SIZE 22
#define TABLE_WIDTH (ADDRESS_COL_SIZE + SRC_CODE_SIZE + MACHINE_CODE_SIZE)

#define REG_WIN_WIDTH 41
#define REG_WIN_LENGTH 7
#define PER_REG_SIZE 10
#define TOTAL_REGISTERS 7

#define FLAG_WIN_WIDTH 41
#define FLAG_WIN_LENGTH 5
#define PER_FLAG_SIZE 8
#define TOTAL_FLAGS 5

#define BTN_LENGTH 3
#define RUN_BTN_SIZE (ADDRESS_COL_SIZE + SRC_CODE_SIZE - 1)
#define INSPECT_BTN_SIZE (TABLE_WIDTH - RUN_BTN_SIZE - 2)

std::string to_hex(size_t value, size_t hex_digits);

// =============================================================================
//                       Interface Class for UI
// =============================================================================
class InterfaceUI {
 public:
    virtual void add_ui() = 0;
    void initialize();
    CursesWindow& get_window();

    void setDimension(const size_t length, const size_t width);
    void setStartPosition(const size_t start_y, const size_t start_x);
    void setHeader(const std::string& header);

 protected:
    CursesWindow __window;
    size_t __length, __width, __start_y, __start_x;
    std::string __header;
};

// =============================================================================
//                       Table Column UI Class
// =============================================================================
class TableUI : public InterfaceUI {
 public:
    void add_ui() final;
};

// =============================================================================
//                       Flags UI Class
// =============================================================================
class FlagsUI : public InterfaceUI {
 public:
    void add_ui() final;

 private:
    const std::vector<std::string> flag_names = {"S", "Z", "AC", "P", "CY"};
};

// =============================================================================
//                       Registers UI Class
// =============================================================================
class RegistersUI : public InterfaceUI {
 public:
    void add_ui() final;
    void refresh(const std::string& reg, const uint8_t value);

 private:
    const std::vector<std::string> register_names =
        {"A", "B", "C", "D", "E", "H", "L"};
};

// =============================================================================
//                       Buttons UI Class
// =============================================================================
class ButtonUI : public InterfaceUI {
 public:
    void add_ui() final;
};

// =============================================================================
//                       Builder Class for UI Elements
// =============================================================================
enum class UIType {
    Button,
    Flags,
    Registers,
    Table,
};

class UIBuilder {
 private:
    explicit UIBuilder(std::unique_ptr<InterfaceUI> ui);

 public:
    static UIBuilder create(const UIType& ui_type);

    UIBuilder& setDimension(const size_t length, const size_t width);
    UIBuilder& setStartPosition(const size_t start_y, const size_t start_x);
    UIBuilder& setHeader(const std::string& header);
    // std::unique_ptr<InterfaceUI> build();

    template<typename T>
    std::unique_ptr<T> build() {
        __ui->add_ui();
        return std::unique_ptr<T>(static_cast<T*>(__ui.release()));
    }

 private:
    std::unique_ptr<InterfaceUI> __ui;
};

#endif  // __UI_BUILDER_HPP__
