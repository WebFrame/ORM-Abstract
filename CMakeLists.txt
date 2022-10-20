# WebFrame main CMakeLists with all sub projects
cmake_minimum_required (VERSION 3.14)

project ("WebFrame" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

option(USE_ORM "Use ORM" ON)
option(USE_MYSQL "Use MySQL Client" ON)
option(RUN_TESTS "Run unit & integration tests" ON)

configure_file(WebFrame.hpp.in WebFrame.hpp)

# Include sub-projects.
if(USE_ORM)
    add_subdirectory("lib/ORM")
    list(APPEND EXTRA_LIBS ORM)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib")
endif()

if(USE_MYSQL)
    execute_process(COMMAND mysql_config --cflags
        OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND mysql_config --libs
        OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_compile_options(${MYSQL_CFLAGS})
    list(APPEND EXTRA_LIBS ${MYSQL_LIBS})
endif()

if(RUN_TESTS)
    add_subdirectory("tests/ORM")
endif()

# Generate example source code
add_executable(Example example/ORM.cpp)
target_link_libraries(Example PUBLIC ${EXTRA_LIBS})
target_include_directories(Example PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

#include(CPack)
