set(LIB common)

add_library(${LIB} STATIC
    src/host_event.cpp
    src/host_frame.cpp
    src/host_sys.cpp
    src/host_time.cpp
    src/mem_sys.cpp
    src/mem_zone.cpp
)

target_include_directories(${LIB} PRIVATE ${PACKAGE_BINARY_DIR})
target_include_directories(${LIB} PUBLIC "./include")
target_link_libraries(${LIB} PRIVATE client server ${SDL2_LIB})
