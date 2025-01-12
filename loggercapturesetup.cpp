#include "loggercapturesetup.h"
#include "ui_loggercapturesetup.h"

LoggerCaptureSetup::LoggerCaptureSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggerCaptureSetup)
{
    ui->setupUi(this);

    fillMessageIds();
    fillDestIds();

    updateUi();
}

LoggerCaptureSetup::~LoggerCaptureSetup()
{
    delete ui;
}

void LoggerCaptureSetup::fillDestIds(void) {
    ui->cbDestId->blockSignals(true);
    ui->cbDestId->addItem(QString("None"));
    for(uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("%1").arg(i, 2, 16, QChar('0')).toUpper();
        ui->cbDestId->addItem(str);
    }
    ui->cbDestId->setCurrentIndex(0);
    ui->cbDestId->blockSignals(false);
}

void LoggerCaptureSetup::fillMessageIds(void) {
    ui->cbMsgId->blockSignals(true);
    ui->cbMsgId->addItem(QString("None"));
    for(uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("%1").arg(i, 2, 16, QChar('0')).toUpper();
        ui->cbMsgId->addItem(str);
    }
    ui->cbMsgId->addItem(QString("0x%1").arg(0xff, 0, 16, QChar('0')).toUpper());
    ui->cbMsgId->blockSignals(false);
    ui->cbDestId->setCurrentIndex(0);
}

void LoggerCaptureSetup::updateUi()
{

    if (ui->cbCaptureFilter->isChecked()) {
        ui->cbMsgId->setEnabled(true);
        ui->cbDestId->setEnabled(true);
    } else {
        ui->cbMsgId->setEnabled(false);
        ui->cbDestId->setEnabled(false);
    }

}

bool LoggerCaptureSetup::getCaptureFilter()
{
    return ui->cbCaptureFilter->isChecked();
}

int LoggerCaptureSetup::getFilterDstId()
{
    return ui->cbDestId->currentText().toUInt(NULL, 16);
}

int LoggerCaptureSetup::getFilterMsgId()
{
    return ui->cbMsgId->currentText().toUInt(NULL, 16);
}

void LoggerCaptureSetup::on_btnOK_clicked()
{
    this->accept();
}

void LoggerCaptureSetup::on_btnCancel_clicked()
{
    this->close();
}

void LoggerCaptureSetup::on_cbCaptureFilter_stateChanged(int arg1)
{
    updateUi();
}
