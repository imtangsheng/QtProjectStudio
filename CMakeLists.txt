cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "e:/Qt/6.7.2/msvc2019_64") # Qt Kit Dir
# 启用 Qt 的自动 moc、rcc 和 uic
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${PROJECT_SOURCE_DIR}/include)

# add_subdirectory(sdk)
# add_subdirectory(client)
# add_subdirectory(server)
# add_subdirectory(mylib)
add_subdirectory(devices)

add_subdirectory(include)
