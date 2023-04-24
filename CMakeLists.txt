cmake_minimum_required(VERSION 3.25)
project(compresso C)

set(CMAKE_C_STANDARD 11)

add_executable(compresso main.c cli/cli_utils.c cli/cli_utils.h cli/main.c core/huffman.c core/huffman.h core/lzw.c core/lzw.h core/compression.c core/compression.h ui/main.c ui/ui_utils.c ui/ui_utils.h)
