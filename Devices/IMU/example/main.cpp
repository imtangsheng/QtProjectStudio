/*
    Title   :   IMU Server
    Author  :   Tang
    Language:   C++
    Date    :   2024-09
    Description: 这个是惯导设备的无界面程序
*/
#include "log.h"
#include "serialport/IMU/device_imu.h"
#include <QScopedPointer>

int main()
{

    spdlog::enable_backtrace(32); //启用堆栈跟踪(backtrace)功能
    spdlog::set_level(spdlog::level::debug);
    // spdlog 使用 %s 来表示源文件名，%# 来表示行号。
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^---%l---%$] [%s:%#] [%t] %v");
    SPDLOG_INFO("This is a IMU Server.");
    //spdlog::info("{}:{} - This is a IMU Server.", __FILE__, __LINE__);
    // 使用示例
    setup_logging();
    // 或者手动提供源位置信息 对于 MSVC，我们使用 __FUNCTION__ 而不是 SPDLOG_FUNCTION
    spdlog::log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, "This is another info message");

    spdlog::debug("This is a debug message");
    spdlog::info("This is an info message");
    spdlog::warn("This is a warning message");
    spdlog::error("This is an error message");


    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is a debug message {}",123);
    LOG_WARN("This is a warn message");
    LOG_ERROR("This is a IMU Server.");

    QScopedPointer<DeviceIMU> imu(new DeviceIMU());

    imu->init();
    imu->start();
    imu->stop();
    imu->quit();

    spdlog::info("The is End of Test!");
    return 0;
}