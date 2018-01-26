// Copyright (c) 2017 The Khronos Group Inc.
// Copyright (c) 2017 Valve Corporation
// Copyright (c) 2017 LunarG Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIBSPIRV_OPT_LOCAL_SSA_ELIM_PASS_H_
#define LIBSPIRV_OPT_LOCAL_SSA_ELIM_PASS_H_

#include <algorithm>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "basic_block.h"
#include "def_use_manager.h"
#include "mem_pass.h"
#include "module.h"

namespace spvtools {
namespace opt {

// See optimizer.hpp for documentation.
class LocalMultiStoreElimPass : public MemPass {
  using cbb_ptr = const ir::BasicBlock*;

 public:
  using GetBlocksFunction =
      std::function<std::vector<ir::BasicBlock*>*(const ir::BasicBlock*)>;

  LocalMultiStoreElimPass();
  const char* name() const override { return "eliminate-local-multi-store"; }
  Status Process(ir::IRContext* c) override;

  ir::IRContext::Analysis GetPreservedAnalyses() override {
    return ir::IRContext::kAnalysisDefUse |
           ir::IRContext::kAnalysisInstrToBlockMapping;
  }

 private:
  // Initialize extensions whitelist
  void InitExtensions();

  // Return true if all extensions in this module are allowed by this pass.
  bool AllExtensionsSupported() const;

  // Remove remaining loads and stores of function scope variables only
  // referenced with non-access-chain loads and stores from function |func|.
  // Insert Phi functions where necessary. Running LocalAccessChainRemoval,
  // SingleBlockLocalElim and SingleStoreLocalElim beforehand will improve
  // the runtime and effectiveness of this function.
  bool EliminateMultiStoreLocal(ir::Function* func);

  void Initialize(ir::IRContext* c);
  Pass::Status ProcessImpl();

  // Extensions supported by this pass.
  std::unordered_set<std::string> extensions_whitelist_;
};

}  // namespace opt
}  // namespace spvtools

#endif  // LIBSPIRV_OPT_LOCAL_SSA_ELIM_PASS_H_
