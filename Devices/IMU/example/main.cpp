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
#include <QCoreApplication>

#include <csignal>
#include <windows.h>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    spdlog::enable_backtrace(32); //启用堆栈跟踪(backtrace)功能
    spdlog::set_level(spdlog::level::debug);
    // spdlog 使用 %s 来表示源文件名，%# 来表示行号。
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^---%l---%$] [%s:%#] [%t] %v");
    SPDLOG_DEBUG("This is a IMU Server Test.");
    //spdlog::info("{}:{} - This is a IMU Server.", __FILE__, __LINE__);
    // 使用示例
    setup_logging();

    QString portName = "COM10";
    QScopedPointer<DeviceIMU> imu(new DeviceIMU(nullptr,portName));
    if(!imu->isOpen()){
        qWarning()<<"IMU is not open!";
    }

    qDebug()<<"init "<<imu->init();

    qDebug()<<imu->availablePorts();
    qDebug()<<imu->availablePorts().contains(portName);

    qDebug()<<"start "<<imu->start();
    // imu->stop();
    // imu->quit();

    SetConsoleCtrlHandler([](DWORD event){
        SPDLOG_INFO("Received signal: EVENT");
        switch (event) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            QCoreApplication::quit(); // 处理信号
            return TRUE;  // 返回 TRUE 表示信号已处理
        default:
            QCoreApplication::quit();
            return FALSE;
        }
    },TRUE);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        // 确保在程序退出前保存数据
        SPDLOG_INFO("The is End of main.cpp!");
        qDebug()<<"stop "<<imu->stop();
        qDebug()<<"quit "<<imu->quit();
        imu->deleteLater();
    });
    
    return app.exec();
}
