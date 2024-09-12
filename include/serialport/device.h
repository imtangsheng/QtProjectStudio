#ifndef DEVICE_H
#define DEVICE_H

#include "serialport/serialport.h"

using StateType = int; //int 类型的默认值是 0。

class DeviceBase
{
public:
    DeviceBase() = default;
    virtual ~DeviceBase() = default;
    virtual Result init() = 0; //初始化
    virtual Result start() = 0; //启动
    virtual Result stop() = 0; //停止
    virtual Result pause() {return false;}; //暂停
    // virtual Result resume(){}; //恢复
    // virtual Result reset(){}; // 重置
    // virtual Result update(){}; // 更新
    virtual Result quit() = 0; //退出

protected:
    String name = "default";

    State<StateType> status;
    
    // std::map<, std::function<void(QByteArray)>> map_cmd;	//发送命令的映射表

};

class Command 
{
public:
    explicit Command(DeviceBase* dev = nullptr) : dev(dev) {}
    virtual ~Command() = default;
    virtual Result parseByte(QByteArray packet) = 0;

protected:
    DeviceBase *dev; // 设备
    mutable DataCode code = 0x00;
};

class SerialDevice : public SerialPortTemplate,DeviceBase
{
public:
    SerialDevice(SerialPortTemplate* parent = nullptr,QString portName = NULL) : SerialPortTemplate(parent,portName){}
protected:
};
#endif // DEVICE_H
