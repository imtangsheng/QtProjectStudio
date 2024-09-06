#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "common.h"

/**
 * 多面阵-车载 通讯手册 1.2  电子组 2022-04-18
 * 数据格式：16 进制
 * 数据帧格式：（8 位数据位，1 位停止位，无校验，默认波特率 921600）
 * 标识符（2 byte） 41 54
 * 本机地址 （1 byte） 64
 * 功能码（1 byte）
 * 数据格式：16 进制
 * 数据（n byte）
 * CRC 校验（2 byte）
 * 结尾（2 byte）0D 0A
 * 
 * 标识符：固定为 41 54
 * 本机地址：固定为 64
 * 功能码：每个功能有不同的功能码
 * 数据：数据的长度可以为 0 到 n 字节，长度为 0 时直接跳过这一位
 * CRC 校验：采用 crc16 的方式进行校验，校验的字节为 CRC 之前的所有字节，低字节在前，高字节在后；手册结尾另附说明。
 * 结尾：固定为 0D 0A
 * 
 */
// 常量定义
constexpr uint16_t DATA_HEADER = 0x4154;       // 数据帧标识符
constexpr uint8_t DATA_LOCAL_ADDRESS = 0x64;   // 本机地址
constexpr uint16_t DATA_END = 0x0D0A;          // 数据帧结尾

using DataCode = uint8_t;

// CRC16 校验类
class CRC16 {
public:
    // 计算 CRC16 校验值
    static uint16_t calculate(const std::vector<uint8_t>& data) {
        uint16_t crc = 0xFFFF;
        for (uint8_t byte : data) {
            crc ^= static_cast<uint16_t>(byte);
            for (int i = 0; i < 8; ++i) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        return ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
    }
};

inline unsigned int CRC16_2(unsigned char *buf, int len)
{
    int pos,i;
    unsigned int crc = 0xFFFF;
    for (pos = 0; pos < len; pos++)
    {
        crc ^= (unsigned int)buf[pos]; // XOR byte into least sig. byte of crc
        for (i = 8; i != 0; i--) // Loop over each bit
        {
            if ((crc & 0x0001) != 0) // If the LSB is set
            {
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else // Else LSB is not set
            {
                crc >>= 1; // Just shift right
            }
        }
    }
    //高低字节转换
    crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
    return crc;
}


class API_EXPORT DeviceBase;



class API_EXPORT SerialPort
{
public:
    SerialPort()  = default;
    ~SerialPort() = default;

    // 禁用拷贝操作
    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    // 启用移动操作
    SerialPort(SerialPort&&) = default;
    SerialPort& operator=(SerialPort&&) = default;


private:
    
};

#endif // SERIALPORT_H
