cmake_minimum_required(VERSION 3.14)

include(CTest)
enable_testing()

add_executable(ORM-Tests main.cpp)
target_include_directories(ORM-Tests PUBLIC "${PROJECT_SOURCE_DIR}/tests/lib")

add_test(NAME Usage COMMAND ORM-Tests)
set_tests_properties(Usage
  PROPERTIES 
  PASS_REGULAR_EXPRESSION "+ [0-9]+)"
  FAIL_REGULAR_EXPRESSION "x [0-9]+)"
)
