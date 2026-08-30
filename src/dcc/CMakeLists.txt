project("dcc" VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(PACKAGE_NAME ${PROJECT_NAME})
set(PACKAGE_TARNAME "dcc")
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")

configure_file(${CONFIG_H_IN} "${CMAKE_CURRENT_BINARY_DIR}/config.h")

add_subdirectory(libs/catch2)

set(DCC_LIB "dcc-lib")
set(DCC_SRC
    src/cc/lexer.cpp
    src/cc/token.cpp
    src/driver/driver.cpp
)
add_library(${DCC_LIB} STATIC ${DCC_SRC})
target_include_directories(${DCC_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} "src/")

add_executable(${PACKAGE_TARNAME} src/main.cpp)
target_link_libraries(${PACKAGE_TARNAME} PRIVATE ${DCC_LIB})

set(DCC_TESTS "dcc-tests")
add_executable(${DCC_TESTS} tests/lexer.cpp)
target_link_libraries(${DCC_TESTS} PRIVATE ${DCC_LIB} Catch2::Catch2WithMain)

if(MSVC)
    set_target_properties(${PACKAGE_TARNAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
    set_target_properties(${DCC_TESTS} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()
