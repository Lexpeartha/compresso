cmake_minimum_required(VERSION 3.25)
project(compresso C)

set(CMAKE_C_STANDARD 11)

include_directories("cli/")
include_directories("core/")
include_directories("ui/")

file(GLOB files CONFIGURE_DEPENDS
        "*.h"
        "*.c"
        "cli/*.h"
        "cli/*.c"
        "core/*.h"
        "core/*.c"
        "ui/*.h"
        "ui/*.c"
        )


add_executable(compresso ${files})

add_definitions(-DUSE_CLI -DUSE_UI)
