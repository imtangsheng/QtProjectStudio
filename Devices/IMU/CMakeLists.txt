# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.14)

# 检查是否作为顶级项目
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # 如果是顶级项目，则定义项目
    project(SerialIMU VERSION 1.0 LANGUAGES CXX)
    set(PROJECT_PATH "../../")
else()
    # 如果是子项目，则只设置一个变量表示项目名称
    set(SERIALIMU_PROJECT_NAME SerialIMU)
    set(PROJECT_PATH ${PROJECT_SOURCE_DIR})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)

# 查找所需的 Qt 组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort)

# 获取当前项目的根目录
get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
include_directories(${PROJECT_PATH}/include)
# 定义库目标
add_library(SerialIMU SHARED
    src/device_imu.cpp
    ${PROJECT_PATH}/include/serialport/IMU/device_imu.h
)
# 定义预处理宏
target_compile_definitions(SerialIMU PRIVATE API_LIBRARY)

# 设置包含目录
target_include_directories(SerialIMU PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 链接 Qt 库
target_link_libraries(SerialIMU PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::SerialPort
)



# 为不同的配置类型设置输出目录（主要用于多配置生成器，如 Visual Studio）
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# 添加示例子目录（可选）
option(BUILD_EXAMPLES "Build the examples" ON)
# option(BUILD_EXAMPLES "Build the examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

# 将动态库 (.so, .dll 或 .dylib) 文件安装到 lib 目录。
# 将静态库 (.a 或 .lib) 文件安装到 lib 目录。
# 将可执行文件 (.exe 或无扩展名) 安装到 bin 目录。
# 将与 SerialIMU 目标相关的头文件安装到 include 目录。
install(TARGETS SerialIMU
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# 将 include/seriallib 目录下的所有文件和子目录,安装到目标系统的 include 目录下。
install(DIRECTORY include/seriallib DESTINATION include)
