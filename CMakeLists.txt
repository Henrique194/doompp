cmake_minimum_required(VERSION 3.31.0)
project("Doom++" VERSION 1.0.0 LANGUAGES C CXX)

# Common Variables Across All Projects
string(TIMESTAMP CURRENT_YEAR "%Y")
set(PACKAGE_COPYRIGHT "Copyright (C) 2025-${CURRENT_YEAR}")
set(PACKAGE_LICENSE "GNU General Public License, version 3")
set(CONFIG_H_IN "${CMAKE_SOURCE_DIR}/cmake/config.h.in")

# Doom C Compiler
add_subdirectory("src/dcc")
# Doom++ Engine Executable
add_subdirectory("src/engine")
# Game Code Project
add_subdirectory("src/game")
