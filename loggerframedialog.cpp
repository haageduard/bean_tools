#include "loggerframedialog.h"
#include "ui_loggerframedialog.h"

LoggerFrameDialog::LoggerFrameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoggerFrameDialog)
{
    ui->setupUi(this);
}

LoggerFrameDialog::~LoggerFrameDialog()
{
    delete ui;
}
