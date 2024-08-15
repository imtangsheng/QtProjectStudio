add_library(mysdk STATIC
    src/mysdk.cpp
    include/mysdk.h
)

target_include_directories(mysdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)