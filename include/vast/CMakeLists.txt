# Copyright (c) 2022-present, Trail of Bits, Inc.

add_subdirectory(ABI)
add_subdirectory(Conversion)
add_subdirectory(Dialect)
add_subdirectory(Interfaces)
add_subdirectory(Target)
add_subdirectory(Translation)
add_subdirectory(Util)

configure_file(Version.hpp.in Version.hpp @ONLY)
