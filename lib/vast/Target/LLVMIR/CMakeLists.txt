add_library(vast_target_llvmir
    Convert.cpp
)

target_link_libraries(vast_target_llvmir
  PRIVATE
    MLIRSupport

    vast::conversion::all
    vast::settings
)

add_library(vast::target_llvmir ALIAS vast_target_llvmir)
