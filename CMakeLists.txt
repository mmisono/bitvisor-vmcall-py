cmake_minimum_required(VERSION 3.0)

project(vmcall)
set(PYBIND11_CPP_STANDARD -std=c++11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")
add_subdirectory(pybind11)

pybind11_add_module(vmcall SHARED vmcall.cc)
