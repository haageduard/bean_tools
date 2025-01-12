#ifndef LOGGERCAPTURESETUP_H
#define LOGGERCAPTURESETUP_H

#include <QDialog>

namespace Ui {
class LoggerCaptureSetup;
}

class LoggerCaptureSetup : public QDialog
{
    Q_OBJECT

public:
    explicit LoggerCaptureSetup(QWidget *parent = nullptr);
    ~LoggerCaptureSetup();
    bool getCaptureFilter();
    int getFilterDstId();
    int getFilterMsgId();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

    void on_cbCaptureFilter_stateChanged(int arg1);

private:
    Ui::LoggerCaptureSetup *ui;
    void fillDestIds();
    void fillMessageIds();
    void updateUi();
};

#endif // LOGGERCAPTURESETUP_H
