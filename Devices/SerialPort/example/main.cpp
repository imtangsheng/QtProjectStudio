#include <QCoreApplication>
#include <QDebug>
#include "seriallib/serialport.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SerialLib::SerialPort serialPort;

    QObject::connect(&serialPort, &SerialLib::SerialPort::dataReceived,
                     [](const QByteArray &data) {
                         qDebug() << "Received data:" << data;
                     });

    QObject::connect(&serialPort, &SerialLib::SerialPort::errorOccurred,
                     [](const QString &error) {
                         qDebug() << "Error:" << error;
                     });

    if (serialPort.open("COM10", 9600)) {
        qDebug() << "Serial port opened successfully";
        serialPort.write("Hello, Serial Port!");
    } else {
        qDebug() << "Failed to open serial port";
    }

    return a.exec();
}
