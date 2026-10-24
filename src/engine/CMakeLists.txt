project("doom++" VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(PACKAGE_NAME "Doom++")
set(PACKAGE_TARNAME ${PROJECT_NAME})
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_STRING "${PACKAGE_NAME} ${PROJECT_VERSION}")
set(PACKAGE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CONFIG_H_IN} "${PACKAGE_BINARY_DIR}/config.h")

find_package(SDL2 CONFIG REQUIRED)
set(SDL2_LIB
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

set(LIBS client common server)
foreach(LIB ${LIBS})
    add_subdirectory(${LIB})
endforeach()
list(APPEND LIBS ${SDL2_LIB})

if(WIN32)
    add_executable(${PACKAGE_TARNAME} WIN32 main.cpp)
else()
    add_executable(${PACKAGE_TARNAME} main.cpp)
endif()
target_include_directories(${PACKAGE_TARNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
if(MSVC)
    set_target_properties(${PACKAGE_TARNAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()
target_link_libraries(${PACKAGE_TARNAME} ${LIBS})
# Engine needs game dll to run.
add_dependencies(${PACKAGE_TARNAME} game)
