#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "deviceconnectdialog.h"
#include "ui_deviceconnectdialog.h"

static Ui::DeviceConnectDialog* wnd;
static QSerialPort* ser;

static void fillDevices(void);

DeviceConnectDialog::DeviceConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceConnectDialog)
{
    ui->setupUi(this);

    wnd = ui;

    fillDevices();
}

DeviceConnectDialog::~DeviceConnectDialog()
{
    delete ui;
}

static void fillDevices(void) {

    QList<QSerialPortInfo> portList;

    portList = QSerialPortInfo::availablePorts();

    QList<QSerialPortInfo>::iterator i;

    int idx = wnd->cbDevice->currentIndex();

    wnd->cbDevice->clear();

    for (i= portList.begin(); i != portList.end(); ++i) {
        wnd->cbDevice->addItem(i->portName());
    }

    if (idx >= portList.count()) {
        idx = 0;
    }

    wnd->cbDevice->setCurrentIndex(idx);
}

static void deviceSendBin(void) {
//    uint8_t data[9] = { 0 };
//    uint8_t len = 0;
//    uint8_t bytes_count = 0;
//    data[0] = (uint8_t)wnd->cbPriority->currentIndex() + 1;
//    data[1] = (uint8_t)wnd->cbMessageId->currentIndex();
//    bytes_count = (uint8_t)wnd->sbBytesCount->value();
//    data[2] = bytes_count;
//    memcpy(&data[3], &bytes[0], bytes_count);
//    len = bytes_count + 3;
//    ser->write((const char*)data, sizeof(data));
//    ser->waitForBytesWritten(-1);
}

static void deviceSend(void) {

//    QString str;
//    if (isSending) {
//        return;
//    }
//    isSending = true;
//    str = QString("%1,%2,%3,%4,")
//              .arg(wnd->cbRetransmission->isChecked() ? 1 : 0)
//              .arg(wnd->cbPriority->currentIndex() + 1)
//              .arg(wnd->cbDestId->currentIndex())
//              .arg(wnd->cbMessageId->currentIndex());
//    uint8_t bytes_count = (uint8_t)wnd->sbBytesCount->value();
//    str.append(QString("%1,").arg(bytes_count));
//    for(uint8_t i = 0; i < bytes_count; i++) {
//        str.append(QString("%1,").arg(bytes[i]));
//    }
//    str.append(char(10));
//    //    qInfo() << str;
//    ser->write(str.toStdString().c_str(), str.size());
//    ser->waitForBytesWritten(-1);
//    isSending = false;
}

//void MainWindow::on_btnRefresh_clicked()
//{
//    fillDevices();
//}

//void MainWindow::on_btnStartStop_clicked()
//{

//    wnd->btnStartStop->setEnabled(false);

//    if (!isStarted) {
//        if (wnd->cbDevice->count() > 0) {
//            ser = new QSerialPort(this);
//            ser->setPortName(wnd->cbDevice->currentText());
//            ser->setBaudRate(QSerialPort::Baud115200);
//            ser->setDataBits(QSerialPort::Data8);
//            ser->setParity(QSerialPort::NoParity);
//            ser->setStopBits(QSerialPort::OneStop);
//            ser->setFlowControl(QSerialPort::NoFlowControl);
//            if (ser->open(QSerialPort::ReadWrite)) {
//                isConnected = true;
//                wnd->btnStartStop->setText("Stop");
//                wnd->cbDevice->setEnabled(false);
//                isStarted = true;
//                connect(ser, SIGNAL(readyRead()), this, SLOT(serialReceived()));
//                //                connect(timer, SIGNAL(timeout()), this, SLOT(timerAlarm()));
//                //                timer->start(ui->sbSendPeriod->value());
//                if (isLiveSend) {
//                    deviceSend();
//                }
//            } else {
//                isConnected = false;
//                QMessageBox::warning(this, "Serial port error", "Could not open serial port");
//            }
//        } else {
//            QMessageBox::warning(this, "Serial port error", "Could not open serial port");
//        }
//    } else {
//        if (isConnected) {
//            ser->close();
//            disconnect(ser, SIGNAL(readyRead()));
//        }
//        //        timer->stop();
//        isConnected = false;
//        //        disconnect(timer, SIGNAL(timeout()));
//        isStarted = false;
//        wnd->cbDevice->setEnabled(true);
//        wnd->btnStartStop->setText("Start");
//    }

//    wnd->btnStartStop->setEnabled(true);

//}

void DeviceConnectDialog::on_btnRefresh_clicked()
{
    fillDevices();
}

void DeviceConnectDialog::on_btnCancel_clicked()
{
    this->close();
}

void DeviceConnectDialog::on_btnOK_clicked()
{
    this->accept();
}

QString DeviceConnectDialog::getSerialPortName() {
    return wnd->cbDevice->currentText();
}
