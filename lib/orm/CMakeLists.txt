# CMakeList.txt : CMake project for ORM, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

add_library(ORM STATIC "ORM.cpp")
target_include_directories(ORM INTERFACE "${PROJECT_SOURCE_DIR}/lib")
set_target_properties(ORM PROPERTIES LINKER_LANGUAGE CXX)

# add_library(ORM-MySQL "mysql.hpp")

# TODO: Add tests and install targets if needed.
install(TARGETS ORM DESTINATION ${PROJECT_BINARY_DIR})
