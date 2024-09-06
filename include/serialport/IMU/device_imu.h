#ifndef DEVICE_IMU_H
#define DEVICE_IMU_H
#include "serialport/device.h"

class IMUCommand;
class API_EXPORT DeviceIMU : protected SerialDevice
{
public:
    DeviceIMU();

    Result init() override; //初始化
    Result start() override; //开始
    Result stop() override; //停止
    Result quit() override; //退出

protected:
    String name = "IMU";
    IMUCommand* cmd;
};


#endif // DEVICE_H
