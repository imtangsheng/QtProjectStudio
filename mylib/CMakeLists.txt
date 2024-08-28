cmake_minimum_required(VERSION 3.10)
# CMake 不允许在子目录中重新定义项目名称。当您在子目录中使用 project() 命令时，它会尝试创建一个新的项目，这可能会与父目录中定义的项目冲突。
# project(MyLibProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加库
add_library(mylib SHARED
    src/mylib.cpp
    include/mylib/mylib.h
)

# 设置库的包含目录
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 设置 MYLIB_EXPORTS 宏
target_compile_definitions(mylib PRIVATE MYLIB_EXPORTS)

# 添加示例可执行文件
add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE mylib)