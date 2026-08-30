cmake_minimum_required(VERSION 3.16)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(CATCH_DEVELOPMENT_BUILD "Build tests, enable warnings, enable Werror, etc" OFF)
option(CATCH_ENABLE_REPRODUCIBLE_BUILD "Add compiler flags for improving build reproducibility" ON)

include(CMakeDependentOption)

# Catch2's build breaks if done in-tree. You probably should not build
# things in tree anyway, but we can allow projects that include Catch2
# as a subproject to build in-tree as long as it is not in our tree.
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

project(Catch2
  VERSION 3.16.0 # CML version placeholder, don't delete
  LANGUAGES CXX
  HOMEPAGE_URL "https://github.com/catchorg/Catch2"
  DESCRIPTION "A modern, C++-native, unit test framework."
)

# Provide path for scripts. We first add path to the scripts we don't use,
# but projects including us might, and set the path up to parent scope.
# Then we also add path that we use to configure the project, but is of
# no use to top level projects.
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CatchConfigOptions)
if(CATCH_DEVELOPMENT_BUILD)
  include(CTest)
endif()

# This variable is used in some subdirectories, so we need it here, rather
# than later in the install block
set(CATCH_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Catch2")

# We have some Windows builds that test `wmain` entry point,
# and we need this change to be present in all binaries that
# are built during these tests, so this is required here, before
# the subdirectories are added.
if(CATCH_TEST_USE_WMAIN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:wmainCRTStartup")
endif()

# Basic paths
set(CATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES_DIR ${CATCH_DIR}/src/catch2)

# We need to bring-in the variables defined there to this scope
add_subdirectory(src)

if(CATCH_DEVELOPMENT_BUILD)
  set(CATCH_ALL_TARGETS ${CATCH_IMPL_TARGETS} ${CATCH_TEST_TARGETS})
  add_warnings_to_targets("${CATCH_ALL_TARGETS}")
  # After we added the noreturn hint to FAIL and SKIP, Clang became
  # extremely good at diagnosing tests that test these macros as being
  # noreturn, but not marked as such. This made the warning useless for
  # our test files.
  add_cxx_flag_if_supported_to_targets("-Wno-missing-noreturn" "${CATCH_TEST_TARGETS}")
endif()
