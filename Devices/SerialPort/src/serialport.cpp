#include "seriallib/serialport.h"

namespace SerialLib {

SerialPort::SerialPort(QObject *parent)
    : QObject(parent), m_serialPort(new QSerialPort(this))
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
}

SerialPort::~SerialPort()
{
    close();
}

bool SerialPort::open(const QString &portName, int baudRate)
{
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    return m_serialPort->open(QIODevice::ReadWrite);
}

void SerialPort::close()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

bool SerialPort::isOpen() const
{
    return m_serialPort->isOpen();
}

bool SerialPort::write(const QByteArray &data)
{
    return m_serialPort->write(data) != -1;
}

QByteArray SerialPort::read(qint64 maxSize)
{
    return m_serialPort->read(maxSize);
}

void SerialPort::handleReadyRead()
{
    QByteArray data = m_serialPort->readAll();
    emit dataReceived(data);
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }
    emit errorOccurred(m_serialPort->errorString());
}

} // namespace SerialLib
