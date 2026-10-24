set(LIB client)

add_library(${LIB} STATIC
    src/cl_sys.cpp
    src/vid_sys.cpp
)

target_include_directories(${LIB} PRIVATE ${PACKAGE_BINARY_DIR})
target_include_directories(${LIB} PUBLIC "./include")
target_link_libraries(${LIB} PRIVATE ${SDL2_LIB})
target_link_libraries(${LIB} PUBLIC common)
