/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __CODE_GRAPH_HPP__
#define __CODE_GRAPH_HPP__

/* standard c++ includes */
#include <memory>
#include <string>
#include <vector>

/* third-party c++ includes */

/* project specific c++ includes */
#include "instructions/command.hpp"

// =============================================================================
//                       CodeLine struct
// =============================================================================
struct CodeLine {
    std::string instruction;
    std::shared_ptr<ICommand> cmd;
};

// =============================================================================
//                       InstrNode struct
// =============================================================================
struct InstrNode {
    std::shared_ptr<ICommand> cmd;
    uint16_t address;
    std::shared_ptr<InstrNode> next;
    std::shared_ptr<InstrNode> next_if_false;
};
typedef std::shared_ptr<InstrNode> InstrNode_h;

// =============================================================================
//                CodeGraph (Graph/LinkedList for instructions)
// =============================================================================
class CodeGraph {
 public:
    explicit CodeGraph(const std::vector<CodeLine>& source_code);

    bool run();
};

#endif  // __CODE_GRAPH_HPP__
