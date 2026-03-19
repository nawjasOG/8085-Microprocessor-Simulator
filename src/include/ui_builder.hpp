/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __UI_BUILDER_HPP__
#define __UI_BUILDER_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/* third-party c++ includes */
#include <ncurses_facade.hpp>

/* project specific c++ include */
#include "utils.hpp"

#define TABLE_LENGTH 25
#define ADDRESS_COL_SIZE 11
#define SRC_CODE_SIZE 18
#define MACHINE_CODE_SIZE 18
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
#define RUN_BTN_SIZE ADDRESS_COL_SIZE
#define INSPECT_BTN_SIZE SRC_CODE_SIZE
#define QUIT_BTN_SIZE MACHINE_CODE_SIZE

// =============================================================================
//                       Interface Class for UI
// =============================================================================
class InterfaceUI : public CursesWindow {
 public:
    virtual void add_ui() = 0;
    void initialize(bool border = true);

    void setDimension(const size_t length, const size_t width);
    void setStartPosition(const size_t start_y, const size_t start_x);
    void setHeader(const std::string& header);

 protected:
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
    void refresh(const std::vector<uint8_t>& flags);

 private:
    const std::vector<std::string_view> flag_names = {
        mpu::SIGN_FLAG, mpu::ZERO_FLAG, mpu::AUXIL_CARRY_FLAG, mpu::PARITY_FLAG,
        mpu::CARRY_FLAG };
};

// =============================================================================
//                       Registers UI Class
// =============================================================================
class RegistersUI : public InterfaceUI {
 public:
    void add_ui() final;
    void refresh(const std::vector<uint8_t>& registers);

 private:
    const std::vector<std::string_view> register_names = {
        mpu::ACCUMULATOR, mpu::REG_B, mpu::REG_C, mpu::REG_D, mpu::REG_E,
        mpu::REG_H, mpu::REG_L };
};

// =============================================================================
//                       Buttons UI Class
// =============================================================================
class ButtonUI : public InterfaceUI {
 public:
    void add_ui() final;
};

// =============================================================================
//                       Edtior UI Class
// =============================================================================
class EditorUI : public TableUI {
 public:
    void update(int ch);
    size_t get_line_number() const;
    size_t get_column_number() const;
    void move_to_next_line();
    void delete_last_char();
    std::string get_line() const;

    static constexpr size_t START_Y = 3, START_X = 2;
};

// =============================================================================
//                       Builder Class for UI Elements
// =============================================================================
enum class UIType {
    Button,
    Flags,
    Registers,
    Table,
    Editor,
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
