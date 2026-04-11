/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __ADC_HPP__
#define __ADC_HPP__

/* standard c++ includes */
#include <string>

/* project specific c++ includes */
#include "command.hpp"

// =============================================================================
//                       ADC Class for ADC instruction
// =============================================================================
class ADC : public ICommand {
 public:
    explicit ADC(const std::string& instruction);

    bool execute(Model& model) final;
    void undo(Model& model) final;

 private:
    void setup_opcode_table() final;
};

#endif  // __ADC_HPP__
