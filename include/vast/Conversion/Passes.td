// Copyright (c) 2022-present, Trail of Bits, Inc.

#ifndef VAST_CONVERSION_PASSES_TD
#define VAST_CONVERSION_PASSES_TD

include "mlir/Pass/PassBase.td"

#ifdef ENABLE_PDLL_CONVERSIONS

// TODO(lukas): Figure out better naming for passes that are implemented in both PDLL and C++
//              (if it becomes more common occurence).
def HLToFunc_pdll : Pass<"vast-hl-to-func-pdll", "mlir::ModuleOp"> {
  let summary = "Lower high-level functions to func.func operations.";
  let description = [{
    Lowers high-level function operations to function dialect.
    Drops high-level information like linkage attributes.

    Work in progress (currently does not work properly).
  }];

  let cppnamespace = pdll;

  let constructor = "vast::pdll::createHLToFuncPass()";
  let dependentDialects = [
    "vast::hl::HighLevelDialect", "mlir::func::FuncDialect",
    "mlir::pdl::PDLDialect", "mlir::pdl_interp::PDLInterpDialect"
  ];
}

#endif // ENABLE_PDLL_CONVERSIONS

def HLFuncToFunc : Pass<"vast-hl-to-func", "mlir::ModuleOp"> {
  let summary = "Lower high-level functions to func.func operations.";
  let description = [{
    Lowers high-level function operations to function dialect.

    Drops high-level information like linkage attributes.
  }];

  let constructor = "vast::createHLFuncToFuncPass()";
  let dependentDialects = [
    "vast::hl::HighLevelDialect", "mlir::func::FuncDialect"
  ];
}

def HLToLLCF : Pass<"vast-hl-to-ll-cf", "mlir::ModuleOp"> {
  let summary = "VAST HL control flow to LL control flow";
  let description = [{
    Transforms high level control flow operations into their low level
    representation.

    This pass is still a work in progress.
  }];

  let constructor = "vast::createHLToLLCFPass()";
  let dependentDialects = ["vast::ll::LowLevelDialect",
                           "vast::hl::HighLevelDialect"];
}


def IRsToLLVM : Pass<"vast-irs-to-llvm", "mlir::ModuleOp"> {
  let summary = "VAST to LLVM Dialect conversion";
  let description = [{
    Converts lowest level VAST operations to LLVM dialect. It is expected
    that module being converted was already lowered by other VAST passes.

    This pass is still a work in progress.
  }];

  let constructor = "vast::createIRsToLLVMPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::ll::LowLevelDialect",
                           "vast::hl::HighLevelDialect"];
}

def CoreToLLVM : Pass<"vast-core-to-llvm", "mlir::ModuleOp"> {
  let summary = "VAST Core dialec to LLVM Dialect conversion";
  let description = [{
    Converts core dialect operations to LLVM dialect.
    }];

  let constructor = "vast::createCoreToLLVMPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::core::CoreDialect",
                           "vast::hl::HighLevelDialect"];
}

def ABIfy : Pass<"vast-abify", "mlir::ModuleOp"> {
  let summary = "Transform functions and apply abi conversion to their type.";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createABIfyPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::ll::LowLevelDialect",
                           "vast::hl::HighLevelDialect", "vast::abi::ABIDialect"];
}

def LowerABI : Pass<"vast-lower-abi", "mlir::ModuleOp"> {
  let summary = "Lower abi operations.";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createLowerABIPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::ll::LowLevelDialect",
                           "vast::hl::HighLevelDialect", "vast::abi::ABIDialect"];
}

def HLToLLGEPs : Pass<"vast-hl-to-ll-geps", "mlir::ModuleOp"> {
  let summary = "Convert hl.member to ll.gep";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createHLToLLGEPsPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::ll::LowLevelDialect"];
}

def HLToLLVars : Pass<"vast-hl-to-ll-vars", "mlir::ModuleOp"> {
  let summary = "Convert hl variables into ll versions.";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createHLToLLVarsPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect", "vast::ll::LowLevelDialect"];
}

def HLStructsToLLVM : Pass<"vast-hl-structs-to-llvm", "mlir::ModuleOp"> {
  let summary = "Transform hl.struct into llvm types without applying ABI conversions.";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createHLStructsToLLVMPass()";
  let dependentDialects = ["mlir::LLVM::LLVMDialect"];
}

def HLEmitLazyRegions : Pass<"vast-hl-to-lazy-regions", "mlir::ModuleOp"> {
  let summary = "Transform hl operations that have short-circuiting into lazy operations.";
  let description = [{
    This pass is still a work in progress.
  }];

  let constructor = "vast::createHLEmitLazyRegionsPass()";
  let dependentDialects = ["vast::core::CoreDialect"];
}

#endif // VAST_CONVERSION_PASSES_TD
