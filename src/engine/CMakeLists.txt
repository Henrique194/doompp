project("doom++" VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(PACKAGE_NAME "Doom++")
set(PACKAGE_TARNAME ${PROJECT_NAME})
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_STRING "${PACKAGE_NAME} ${PROJECT_VERSION}")

configure_file(${CONFIG_H_IN} "${CMAKE_CURRENT_BINARY_DIR}/config.h")

set(SRC_FILES main.cpp)
if(WIN32)
    add_executable(${PACKAGE_TARNAME} ${SRC_FILES})
else()
    add_executable(${PACKAGE_TARNAME} ${SRC_FILES})
endif()
target_include_directories(${PACKAGE_TARNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
if(MSVC)
    set_target_properties(${PACKAGE_TARNAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()
# Engine needs game dll to run.
add_dependencies(${PACKAGE_TARNAME} game)
