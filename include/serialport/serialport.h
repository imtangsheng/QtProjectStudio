#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "common.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QReadWriteLock>
#include <QTimer>

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

class SerialPortTemplate : public QObject
{
    Q_OBJECT
    // 定义属性
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)

public:
    explicit SerialPortTemplate(SerialPortTemplate* parent = nullptr,QString portName = NULL)
        : QObject(parent),m_serialPort(new QSerialPort(this))    {
        //open serial port
        if(!portName.isEmpty()){
            if(availablePorts().contains(portName)){
                m_serialPort->setPortName(portName);
                open();
            }else{
                qWarning()<<tr("找不到名称为%1的串口号!").arg(portName);
            }
        }
        connect(m_serialPort,&QSerialPort::readyRead,this,&SerialPortTemplate::handleReadyRead);
        connect(m_serialPort,&QSerialPort::errorOccurred,this,&SerialPortTemplate::handleError);

        // connect(&parseThread_, &QThread::started, this, &SerialPortTemplate::processBuffer);
        connect(&timer_, &QTimer::timeout, this, &SerialPortTemplate::fixedUpdate);
    }

    virtual ~SerialPortTemplate(){
        close();
    };

    // 添加一个方法来获取父 SerialPortTemplate
    SerialPortTemplate* getParent() const {
        return qobject_cast<SerialPortTemplate*>(parent());
    };

    // 串口是否打开
    bool isOpen() const { return m_serialPort->isOpen(); }
    // 最后一次错误
    QString lastError() const {return m_lastError; }
    void setLastError(const QString error) { m_lastError = error; }

    // 打开串口
    Q_INVOKABLE bool open()
    {
        if (m_serialPort->open(QIODevice::ReadWrite)) {

            sendData("1234");
            emit isOpenChanged();
            return true;
        }
        qWarning()<<tr("打开串口%1失败!").arg(portName);
        return false;
    }

    // 关闭串口
    Q_INVOKABLE void close()
    {
        if (m_serialPort->isOpen()) {
            m_serialPort->close();
            emit isOpenChanged();
        }
    }

    // 发送数据
    Q_INVOKABLE bool sendData(const QByteArray &data)
    {
        if (m_serialPort->isOpen()) {
            return m_serialPort->write(data) == data.size();
        }
        return false;
    }
    
    Q_INVOKABLE QByteArray getData(){
        return m_serialPort->readAll();
    }

    // 获取可用串口列表
    Q_INVOKABLE QStringList availablePorts()
    {
        QStringList ports;
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            ports << info.portName();
        }
        return ports;
    }

signals:
    void isOpenChanged();

protected slots:
    // 解析线程
    virtual void processBuffer(){
        QByteArray bufferCopy;
        {
            QWriteLocker locker(&bufferLock_);
            bufferCopy = buffer_;
            //qDebug()<<"processBuffer sendData:"<<buffer_.toHex().toUpper();
            buffer_.clear();
        }
        sendData(bufferCopy);
    };
    // 定时更新处理
    virtual void fixedUpdate() {
        qDebug()<<"fixedUpdate";
    }

protected:
    QString portName;
    QSerialPort *m_serialPort;
    QString m_lastError;

    QByteArray buffer_;
    mutable QReadWriteLock bufferLock_;

    QTimer timer_;
    virtual void handleReadyRead(){
        // 串口调试工具最快200ms一次发送数据
        // parseByte took 44500 ns (0.044500 ms)
        // handleReadyRead took 277800 ns (0.277800 ms)
        // parseByte took 48400 ns (0.048400 ms)
        // handleReadyRead took 790500 ns (0.790500 ms)
        // parseByte took 46700 ns (0.046700 ms)
        // handleReadyRead took 787300 ns (0.787300 ms)
        // parseByte took 46300 ns (0.046300 ms)
        // handleReadyRead took 784100 ns (0.784100 ms)
        // auto start = std::chrono::high_resolution_clock::now();

        QByteArray data = getData();
        {
            QWriteLocker locker(&bufferLock_);
            buffer_.append(data);
            //qDebug()<<"SerialPortTemplate handleReadyRead:"<<buffer_.toHex().toUpper();
        }
        processBuffer();
        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // double milliseconds = duration.count() / 1e6; // 转换为毫秒
        // qDebug().noquote() << QString("%1 took %2 ns (%3 ms)")
        //                       .arg("handleReadyRead")
        //                       .arg(duration.count())
        //                       .arg(milliseconds, 0, 'f', 6);
        
    };
    virtual void handleError(QSerialPort::SerialPortError error){
        qWarning() << "Serial port error:" << error;
        if (error == QSerialPort::NoError) {
            return;
        }
        m_lastError = m_serialPort->errorString();
    };
private:

};

#endif // SERIALPORT_H
