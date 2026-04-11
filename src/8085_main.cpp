/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <iostream>

/* project specific includes */
#include "app_controller.hpp"
#include "view.hpp"
#include "model.hpp"

void app() {
    ViewUI view;
    Model model;
    AppController app(view, model);
    app.run();
}

int main() {
    try {
        app();
    }
    catch (const std::exception& error) {
        std::cerr << "[fatal error]: " << error.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "an unknown fatal error occurred" << std::endl;
        return 1;
    }
    return 0;
}
