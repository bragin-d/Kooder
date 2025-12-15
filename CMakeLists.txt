# TODO: Actually make a valid CMake setup
# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Enable BOTH C and CXX
project(Kooder C CXX)

# Set a C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the LLVM and Clang packages
# THE FIX: We remove the explicit version "14" to work around
# the "version: unknown" bug in the package's config file.
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Add the necessary include directories
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Define our executable
add_executable(my_tool tool.cpp)

# Link all the libraries required for libtooling.
target_link_libraries(my_tool
  PRIVATE
  clangTooling
  clangASTMatchers
  clangAST
  clangFrontend
  clangBasic
  clangLex
  LLVMSupport
  )