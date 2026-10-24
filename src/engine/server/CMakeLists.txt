set(LIB server)

add_library(${LIB} STATIC
    src/sv_sys.cpp
)

target_include_directories(${LIB} PRIVATE ${PACKAGE_BINARY_DIR})
target_include_directories(${LIB} PUBLIC "./include")
target_link_libraries(${LIB} PUBLIC common)
