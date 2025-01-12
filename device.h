#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "beanframe.h"

class Device: public QObject
{

    Q_OBJECT

public:
    Device();
    ~Device();
    void connect(QString serialPortName);
    void disconnect();
    bool isConnected();
    void send(QString str);
private slots:
    void serialReceived();
//    void serialPortEnumerated();
//    void deviceConnected();
//    void deviceDisconnected();
signals:
    void beanFrameReceived(BeanFrame* frame);
    void beanFrameTransmitted(bool ack);
    void beanFrameTransmitError(void);
private:
    QSerialPort* ser;
    bool connected;
    QByteArray rxBytes;
};

#endif // DEVICE_H
