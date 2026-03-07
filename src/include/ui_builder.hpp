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

#define REG_WIN_WIDTH 41
#define REG_WIN_LENGTH 7
#define PER_REG_SIZE 10
#define TOTAL_REGISTERS 7

#define FLAG_WIN_WIDTH 41
#define FLAG_WIN_LENGTH 5
#define PER_FLAG_SIZE 8
#define TOTAL_FLAGS 5

std::string to_hex(size_t value, size_t hex_digits);

class InterfaceUI {
 public:
    virtual void add_ui() = 0;
    void initialize();

    void setDimension(const size_t length, const size_t width);
    void setStartPosition(const size_t start_y, const size_t start_x);

 protected:
    CursesWindow __window;
    size_t __length, __width, __start_y, __start_x;
};

class FlagsUI : public InterfaceUI {
 public:
    void add_ui() final;

 private:
    const std::vector<std::string> flag_names = {"S", "Z", "AC", "P", "CY"};
};

class RegisterUI : public InterfaceUI {
 public:
    void add_ui() final;
    void refresh(const std::string& reg, const uint8_t value);

 private:
    const std::vector<std::string> register_names =
        {"A", "B", "C", "D", "E", "H", "L"};
};

enum class UIType {
    Flags,
    Registers,
};

class UIBuilder {
 private:
    explicit UIBuilder(std::unique_ptr<InterfaceUI> ui);

 public:
    static UIBuilder create(const UIType& ui_type);

    UIBuilder& setDimension(const size_t length, const size_t width);
    UIBuilder& setStartPosition(const size_t start_y, const size_t start_x);
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
