#ifndef LOGGERFRAMEDIALOG_H
#define LOGGERFRAMEDIALOG_H

#include <QDialog>

namespace Ui {
class LoggerFrameDialog;
}

class LoggerFrameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoggerFrameDialog(QWidget *parent = nullptr);
    ~LoggerFrameDialog();

private:
    Ui::LoggerFrameDialog *ui;
};

#endif // LOGGERFRAMEDIALOG_H
