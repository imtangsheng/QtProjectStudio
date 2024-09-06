#include "serialport/IMU/device_imu.h"

class IMUCommand : public Command
{
private:
    /* data */
public:
    explicit IMUCommand(DeviceIMU& device) : dev(&device) {} // 使用引用
private:
    DeviceIMU* dev; // 改为原始指针或引用

};

DeviceIMU::DeviceIMU()
{
    qDebug() << "DeviceIMU::DeviceIMU()";
    cmd = new IMUCommand(*this);
}

Result DeviceIMU::init()
{
    qDebug() << "DeviceIMU::init()";
    return Result();
}

Result DeviceIMU::start()
{
    qDebug() << "DeviceIMU::start()";
    return Result();
}

Result DeviceIMU::stop()
{
    qDebug() << "DeviceIMU::stop()";
    return Result();
}

Result DeviceIMU::quit()
{
    qDebug() << "DeviceIMU::quit()";
    return Result();
}
