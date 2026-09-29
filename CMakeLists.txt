cmake_minimum_required(VERSION 3.10)
project(impede LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -ffunction-sections -fdata-sections -Wl,--gc-sections)
add_compile_options(
        -Wall -Wextra
        -Wdouble-promotion
        -Wformat-security
        -Wformat=2 -Wformat-overflow=2
        -Wformat-truncation=2
        -fno-common -fstack-usage)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wmissing-prototypes>)
# From https://github.com/lvgl/lv_port_pc_vscode/blob/master/CMakeLists.txt
add_compile_options(-pedantic-errors
        -Wclobbered
        -Wdeprecated
        -Wempty-body
        -Wmaybe-uninitialized
        -Wpointer-arith
        -Wmultichar
        -Wno-pedantic # ignored for now, we convert functions to pointers for properties table.
        -Wreturn-type
        -Wshadow
        -Wshift-negative-value
        -Wsizeof-pointer-memaccess
        -Wtype-limits
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wfloat-conversion
        -Wstrict-aliasing
)

include_directories(include)
add_executable(impede src/main.cpp)

# According to AI: GCC 15 provides but on Windows its terminal implementation is located  in the separate libstdc++exp library, which is not linked by default.
if (MINGW)
    target_link_libraries(impede PRIVATE stdc++exp)
endif ()

