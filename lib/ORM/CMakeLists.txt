cmake_minimum_required (VERSION 3.14)

add_library(ORM STATIC "ORM.cpp")
target_include_directories(ORM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")
set_target_properties(ORM PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS ORM DESTINATION ${PROJECT_BINARY_DIR})
