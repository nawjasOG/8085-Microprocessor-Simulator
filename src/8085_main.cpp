/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <iostream>

/* project specific includes */
#include "include/app_controller.hpp"
#include "include/view.hpp"

int main() {
    ViewUI view;
    AppController app(view);
    try {
        app.run();
    }
    catch (const std::exception& error) {
        std::cout << "[fatal error]: " << error.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "an unknown fatal error occurred" << std::endl;
        return 1;
    }
    return 0;
}
