cmake_minimum_required(VERSION 3.10)
project(lab_1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

add_executable (
    lab_1
    src/matrix.c
    src/int_field_info.c
    main.c
)

target_include_directories(lab_1 PRIVATE include)