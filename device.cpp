#include "device.h"

Device::Device()
{
    connected = false;
}

Device::~Device()
{
}

void Device::serialReceived()
{
    QByteArray ba;
    ba = ser->readAll();
    for (int i = 0; i < ba.length(); i++) {
        if (ba[i] != '\n') {
            rxBytes.append(ba[i]);
        } else {
            QList<QByteArray> bytes = rxBytes.split(' ');
            qInfo(rxBytes);
            if (bytes.at(0).at(0) == 'R') {
                BeanFrame frame;
                frame.setPri((bytes.at(1).toUInt(NULL, 16)) >> 4);
                frame.setMl((bytes.at(1).toUInt(NULL, 16)) & 0x0F);
                frame.setDstId(bytes.at(2).toUInt(NULL, 16));
                frame.setMsgId(bytes.at(3).toUInt(NULL, 16));
                for(int i = 0; i < frame.getMl() - 2; i++) {
                    frame.setData(i, bytes.at(4 + i).toUInt(NULL, 16));
                }
                frame.setCrc(bytes.at(frame.getMl() + 2).toUInt(NULL, 16));
                frame.setAck(bytes.at(frame.getMl() + 4).toUInt(NULL, 16));
                emit beanFrameReceived(&frame);
                rxBytes.clear();
            } else if (bytes.at(0).at(0) == 'W') {
                QString str = QString(bytes.at(1));
                if (QString::compare(str, QString("NOACK")) == 0) {
                    emit beanFrameTransmitted(false);
                } else if (QString::compare(str, QString("ACK")) == 0) {
                    emit beanFrameTransmitted(true);
                } else if (QString::compare(str, QString("ERR")) == 0) {
                    emit beanFrameTransmitError();
                }
                rxBytes.clear();
            } else {
                rxBytes.clear();
            }
        }
    }

}

void Device::connect(QString serialPortName) {
    ser = new QSerialPort(this);
    ser->setPortName(serialPortName);
    ser->setBaudRate(QSerialPort::Baud115200);
    ser->setDataBits(QSerialPort::Data8);
    ser->setParity(QSerialPort::NoParity);
    ser->setStopBits(QSerialPort::OneStop);
    ser->setFlowControl(QSerialPort::NoFlowControl);
    ser->setReadBufferSize(1024);
    if (ser->open(QSerialPort::ReadWrite)) {
        connected = true;
        QObject::connect(ser, SIGNAL(readyRead()), this, SLOT(serialReceived()));
     } else {
        connected = false;
     }
}

void Device::disconnect() {
    ser->close();
    QObject::disconnect(ser, SIGNAL(readyRead()));
    connected = false;
}


bool Device::isConnected() {
    return connected;
}

void Device::send(QString str)
{
    ser->write(str.toStdString().c_str(), str.size());
    ser->waitForBytesWritten(-1);
}
