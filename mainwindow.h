#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "device.h"
#include "loggermodel.h"
#include "loggergroupmodel.h"
#include "toggledbframemodel.h"
#include "playermodel.h"
#include "dashboardmetricmodel.h"
#include "dashboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadDbFrame();
    
    void initBeanFrame();
    
private slots:
    void on_cbByte17_toggled(bool checked);
    void on_cmByte16_toggled(bool checked);
    void on_cmByte15_toggled(bool checked);
    void on_cmByte14_toggled(bool checked);
    void on_cmByte13_toggled(bool checked);
    void on_cmByte12_toggled(bool checked);
    void on_cmByte11_toggled(bool checked);
    void on_cmByte10_toggled(bool checked);
    void on_cbByte27_toggled(bool checked);
    void on_cmByte26_toggled(bool checked);
    void on_cmByte25_toggled(bool checked);
    void on_cmByte24_toggled(bool checked);
    void on_cmByte23_toggled(bool checked);
    void on_cmByte22_toggled(bool checked);
    void on_cmByte21_toggled(bool checked);
    void on_cmByte20_toggled(bool checked);
    void on_cbByte37_toggled(bool checked);
    void on_cmByte36_toggled(bool checked);
    void on_cmByte35_toggled(bool checked);
    void on_cmByte34_toggled(bool checked);
    void on_cmByte33_toggled(bool checked);
    void on_cmByte32_toggled(bool checked);
    void on_cmByte31_toggled(bool checked);
    void on_cmByte30_toggled(bool checked);
    void on_cbByte47_toggled(bool checked);
    void on_cmByte46_toggled(bool checked);
    void on_cmByte45_toggled(bool checked);
    void on_cmByte44_toggled(bool checked);
    void on_cmByte43_toggled(bool checked);
    void on_cmByte42_toggled(bool checked);
    void on_cmByte41_toggled(bool checked);
    void on_cmByte40_toggled(bool checked);
    void on_cbByte57_toggled(bool checked);
    void on_cmByte56_toggled(bool checked);
    void on_cmByte55_toggled(bool checked);
    void on_cmByte54_toggled(bool checked);
    void on_cmByte53_toggled(bool checked);
    void on_cmByte52_toggled(bool checked);
    void on_cmByte51_toggled(bool checked);
    void on_cmByte50_toggled(bool checked);
    void on_cbByte67_toggled(bool checked);
    void on_cmByte66_toggled(bool checked);
    void on_cmByte65_toggled(bool checked);
    void on_cmByte64_toggled(bool checked);
    void on_cmByte63_toggled(bool checked);
    void on_cmByte62_toggled(bool checked);
    void on_cmByte61_toggled(bool checked);
    void on_cmByte60_toggled(bool checked);
    void on_btnClearBytes_clicked();
    void serialReceived();
    void timerAlarm();
    void on_cbMessageId_currentIndexChanged(int index);
    void on_cbMessageId_currentTextChanged(const QString &arg1);
    void on_btnSetBytes_clicked();
    void on_btnSetBits1_clicked();
    void on_btnSetBits2_clicked();
    void on_btnSetBits3_clicked();
    void on_btnSetBits4_clicked();
    void on_btnSetBits5_clicked();
    void on_btnSetBits6_clicked();
    void on_btnClearBits1_clicked();
    void on_btnClearBits2_clicked();
    void on_btnClearBits3_clicked();
    void on_btnClearBits4_clicked();
    void on_btnClearBits5_clicked();
    void on_btnClearBits6_clicked();

    void on_cbDestId_currentTextChanged(const QString &arg1);

    void on_cbPriority_currentTextChanged(const QString &arg1);

    void on_btnSendBytes_clicked();

    void on_cbByte77_toggled(bool checked);

    void on_cbByte76_toggled(bool checked);

    void on_cbByte75_toggled(bool checked);

    void on_cbByte74_toggled(bool checked);

    void on_cbByte73_toggled(bool checked);

    void on_cbByte72_toggled(bool checked);

    void on_cbByte71_toggled(bool checked);

    void on_cbByte70_toggled(bool checked);

    void on_cbByte87_toggled(bool checked);

    void on_cbByte86_toggled(bool checked);

    void on_cbByte85_toggled(bool checked);

    void on_cbByte84_toggled(bool checked);

    void on_cbByte83_toggled(bool checked);

    void on_cbByte82_toggled(bool checked);

    void on_cbByte81_toggled(bool checked);

    void on_cbByte80_toggled(bool checked);

    void on_cbByte97_toggled(bool checked);

    void on_cbByte96_toggled(bool checked);

    void on_cbByte95_toggled(bool checked);

    void on_cbByte94_toggled(bool checked);

    void on_cbByte93_toggled(bool checked);

    void on_cbByte92_toggled(bool checked);

    void on_cbByte91_toggled(bool checked);

    void on_cbByte90_toggled(bool checked);

    void on_cbByte107_toggled(bool checked);

    void on_cbByte106_toggled(bool checked);

    void on_cbByte105_toggled(bool checked);

    void on_cbByte104_toggled(bool checked);

    void on_cbByte103_toggled(bool checked);

    void on_cbByte102_toggled(bool checked);

    void on_cbByte101_toggled(bool checked);

    void on_cbByte100_toggled(bool checked);

    void on_cbByte117_toggled(bool checked);

    void on_cbByte116_toggled(bool checked);

    void on_cbByte115_toggled(bool checked);

    void on_cbByte114_toggled(bool checked);

    void on_cbByte113_toggled(bool checked);

    void on_cbByte112_toggled(bool checked);

    void on_cbByte111_toggled(bool checked);

    void on_cbByte110_toggled(bool checked);

    void on_btnSetBits7_clicked();

    void on_btnSetBits8_clicked();

    void on_btnSetBits9_clicked();

    void on_btnSetBits10_clicked();

    void on_btnSetBits11_clicked();

    void on_btnClearBits7_clicked();

    void on_btnClearBits8_clicked();

    void on_btnClearBits9_clicked();

    void on_btnClearBits10_clicked();

    void on_btnClearBits11_clicked();

    void on_sbBytesCount_valueChanged(int arg1);

    void on_cbPriority_currentIndexChanged(int index);

    void on_cbDestId_currentIndexChanged(int index);

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void beanFrameReceived(BeanFrame* frame);

    void on_btnLoggerClear_clicked();

    void on_btnLoggerStart_clicked();

    void on_tblLogger_doubleClicked(const QModelIndex &index);

    void on_tblLoggerGroup_clicked(const QModelIndex &index);

    void on_cmLogFilter_currentIndexChanged(int index);

    void on_tbDatabase_clicked();

    void on_btnAddFrame_clicked();

    void on_btnDeleteFrame_clicked();

    void on_actionNewDatabase_triggered();

    void on_tbToggleDbFrame_clicked(const QModelIndex &index);

    void on_actionSaveDatabase_triggered();

    void on_actionOpenDatabase_triggered();

    void on_btnSaveDump_clicked();

    void on_btnOpenDump_clicked();

    void on_btnSendToPlayer_clicked();

    void on_btnPlay_clicked();

    void on_btnNext_clicked();

    void on_btnPrev_clicked();

    void on_actionAbout_triggered();

    void on_cbByte17_stateChanged(int arg1);

    void on_cbByte17_clicked();

    void on_cbToggleBitPickerMode_toggled(bool checked);

    void on_btnPlayerClear_clicked();

    void on_btnOpenDashboard_clicked();        

private:
    Ui::MainWindow *ui;
    Device* device;
    QTimer* timer;
    QTimer* timeoutTimer;
    LoggerModel* model;
    LoggerGroupModel* groupModel;
    ToggleDbFrameModel* toggleDbFrameModel;
    PlayerModel* playerModel;
    DashboardMetricModel *dashboardMetricModel;
    BeanFrame beanFrame;
    Dashboard dashboard;

    bool isBeanFrameTransmitError;
    bool isBeanFrameTransmitAck;

    qint64 logStartMSec;
    int selectedRowNum;
    bool loggerStarted;
    bool isSending;
    bool canSend;
    void deviceSend(BeanFrame* beanFrame);
    void fillPriority();
    void fillDestIds();
    void fillMessageIds();
    void updateFrameBytes();
    void updateBytes();
    void updateByte(uint8_t byte_num);
    void updateDeviceMenu();
    void applyFilter();
    void lockToggleUI();
    void unlockToggleUI();
    int playerPosition;
    bool isPlaying;
    void onTimerShot(void);
    void onTimerOneShot(void);
    void onTimeout(void);
    void playNext(void);
    void playerStop(void);
    void beanFrameTransmitted(bool ack);
    void beanFrameTransmitError(void);

    void LogFilterApply();
    void setToggleUIEnabled(bool value);

};

#endif // MAINWINDOW_H
