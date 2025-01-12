#ifndef DEVICECONNECTDIALOG_H
#define DEVICECONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class DeviceConnectDialog;
}

class DeviceConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceConnectDialog(QWidget *parent = nullptr);
    ~DeviceConnectDialog();
    QString getSerialPortName();

private slots:
    void on_btnRefresh_clicked();

    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DeviceConnectDialog *ui;
};

#endif // DEVICECONNECTDIALOG_H
