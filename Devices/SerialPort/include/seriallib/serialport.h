// Copyright(c) 2024-southsurvey
// by Tang

#pragma once

#include "serialport_global.h"
#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QByteArray>

namespace SerialLib {

class SERIALLIB_EXPORT SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();

    bool open(const QString &portName, int baudRate);
    void close();
    bool isOpen() const;
    bool write(const QByteArray &data);
    QByteArray read(qint64 maxSize = 0);

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
};

} // namespace SerialLib
