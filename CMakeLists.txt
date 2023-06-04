cmake_minimum_required(VERSION 3.25)
project(compresso C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories("${GTK4_INCLUDE_DIRS}")
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
link_directories("${GTK4_LIBRARY_DIRS}")

add_executable(compresso ${files})

add_definitions(${GTK4_CFLAGS_OTHER})

target_link_libraries(compresso ${GTK4_LIBRARIES})
