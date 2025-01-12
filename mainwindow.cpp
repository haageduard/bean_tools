#include <QDebug>
#include <QList>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceconnectdialog.h"
#include "loggermodel.h"
#include "loggerframedialog.h"
#include "loggercapturesetup.h"
#include "toggledbframemodel.h"
#include "loggerframe.h"

void MainWindow::deviceSend(BeanFrame* frame) {

    QString str;
    if (isSending) {
        return;
    }
    isSending = true;
    str = QString("%1,%2,%3,%4,")
            .arg(0)
              .arg(frame->getPri())
            .arg(frame->getDstId())
            .arg(frame->getMsgId());
    str.append(QString("%1,").arg(frame->getMl() - 2));
    for(uint8_t i = 0; i < frame->getMl() - 2; i++) {
        str.append(QString("%1,").arg(frame->getData(i)));
    }
    str.append(char(10));
    device->send(str);
//    isSending = false;
    qInfo() << "W " << str;
}


void MainWindow::fillPriority(void) {
    ui->cbPriority->blockSignals(true);
    for(uint8_t i = 0; i <= 0xf; i++) {
        QString str;
        str = QString("%1").arg(i, 0, 10);
        ui->cbPriority->addItem(str);
    }
    ui->cbPriority->setCurrentIndex(0);
    ui->cbPriority->blockSignals(false);
}

void MainWindow::fillDestIds(void) {
    ui->cbDestId->blockSignals(true);
    for(uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("%1").arg(i, 2, 16, QChar('0')).toUpper();
        ui->cbDestId->addItem(str);
    }
    ui->cbDestId->addItem(QString("0x%1").arg(0xff, 0, 16));
    ui->cbDestId->setCurrentIndex(0xfe);
    ui->cbDestId->blockSignals(false);
}

void MainWindow::fillMessageIds(void) {
    ui->cbMessageId->blockSignals(true);
    for(uint8_t i = 0; i < 0xff; i++) {
        QString str;
        str = QString("%1").arg(i, 2, 16, QChar('0')).toUpper();
        ui->cbMessageId->addItem(str);
    }
    ui->cbMessageId->addItem(QString("0x%1").arg(0xff, 0, 16, QChar('0')).toUpper());
    ui->cbMessageId->setCurrentIndex(0x83);
    ui->cbMessageId->blockSignals(false);
}



void MainWindow::updateFrameBytes()
{

    uint8_t pri =  (uint8_t)ui->cbPriority->currentIndex();
    beanFrame.setPri(pri);
    uint8_t dstId = (uint8_t)ui->cbDestId->currentIndex();
    beanFrame.setDstId(dstId);
    uint8_t msgId = (uint8_t)ui->cbMessageId->currentIndex();
    beanFrame.setMsgId(msgId);
    uint8_t bytes_count = (uint8_t)ui->sbBytesCount->value();
    beanFrame.setMl(bytes_count + 2);

    QString str;
    str.append(QString("%1 ").arg(pri << 4 | bytes_count + 2, 2, 16, QChar('0')));
    str.append(QString("%1 ").arg(dstId, 2, 16, QChar('0')));
    str.append(QString("%1 ").arg(msgId, 2, 16, QChar('0')));

    for(uint8_t i = 0; i < bytes_count; i++) {
        str.append(QString("%1 ").arg(beanFrame.getData(i), 2, 16, QChar('0')));
    }

    ui->lbFrameBytes->setText(str.toUpper());

    if (toggleDbFrameModel->rowCount() > 0) {
        toggleDbFrameModel->updateFrame(ui->tbToggleDbFrame->currentIndex(), beanFrame);
    }
}

void MainWindow::updateByte(uint8_t byte_num) {
    switch (byte_num) {
        case 0:
            ui->lbByte1->setText(QString("%1").arg(beanFrame.getData(0), 2, 16, QChar('0')).toUpper());
            break;
        case 1:
            ui->lbByte2->setText(QString("%1").arg(beanFrame.getData(1), 2, 16, QChar('0')).toUpper());
            break;
        case 2:
            ui->lbByte3->setText(QString("%1").arg(beanFrame.getData(2), 2, 16, QChar('0')).toUpper());
            break;
        case 3:
            ui->lbByte4->setText(QString("%1").arg(beanFrame.getData(3), 2, 16, QChar('0')).toUpper());
            break;
        case 4:
            ui->lbByte5->setText(QString("%1").arg(beanFrame.getData(4), 2, 16, QChar('0')).toUpper());
            break;
        case 5:
            ui->lbByte6->setText(QString("%1").arg(beanFrame.getData(5), 2, 16, QChar('0')).toUpper());
            break;
        case 6:
            ui->lbByte7->setText(QString("%1").arg(beanFrame.getData(6), 2, 16, QChar('0')).toUpper());
            break;
        case 7:
            ui->lbByte8->setText(QString("%1").arg(beanFrame.getData(7), 2, 16, QChar('0')).toUpper());
            break;
        case 8:
            ui->lbByte9->setText(QString("%1").arg(beanFrame.getData(8), 2, 16, QChar('0')).toUpper());
            break;
        case 9:
            ui->lbByte10->setText(QString("%1").arg(beanFrame.getData(9), 2, 16, QChar('0')).toUpper());
            break;
        case 10:
            ui->lbByte11->setText(QString("%1").arg(beanFrame.getData(10), 2, 16, QChar('0')).toUpper());
            break;
        default:
            break;
    }

    if (device->isConnected()) {
        if (isLiveSend) {
//           deviceSend();
        }
    }
}

void MainWindow::updateDeviceMenu()
{
    if (device->isConnected()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionUpdate_firmware->setEnabled(true);
    } else {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->actionUpdate_firmware->setEnabled(false);
    }
}

void MainWindow::setToggleUIEnabled(bool value)
{
    ui->cbMessageId->setEnabled(value);
    ui->cbDestId->setEnabled(value);
    ui->cbPriority->setEnabled(value);
    ui->sbBytesCount->setEnabled(value);

    ui->btnSendBytes->setEnabled(value);

    ui->btnSetBits1->setEnabled(value);
    ui->btnSetBits2->setEnabled(value);
    ui->btnSetBits3->setEnabled(value);
    ui->btnSetBits4->setEnabled(value);
    ui->btnSetBits5->setEnabled(value);
    ui->btnSetBits6->setEnabled(value);
    ui->btnSetBits7->setEnabled(value);
    ui->btnSetBits8->setEnabled(value);
    ui->btnSetBits9->setEnabled(value);
    ui->btnSetBits10->setEnabled(value);
    ui->btnSetBits11->setEnabled(value);

    ui->btnClearBits1->setEnabled(value);
    ui->btnClearBits2->setEnabled(value);
    ui->btnClearBits3->setEnabled(value);
    ui->btnClearBits4->setEnabled(value);
    ui->btnClearBits5->setEnabled(value);
    ui->btnClearBits6->setEnabled(value);
    ui->btnClearBits7->setEnabled(value);
    ui->btnClearBits8->setEnabled(value);
    ui->btnClearBits9->setEnabled(value);
    ui->btnClearBits10->setEnabled(value);
    ui->btnClearBits11->setEnabled(value);

    ui->btnSetBytes->setEnabled(value);
    ui->btnClearBytes->setEnabled(value);

    ui->cmByte10->setEnabled(value);
    ui->cmByte11->setEnabled(value);
    ui->cmByte12->setEnabled(value);
    ui->cmByte13->setEnabled(value);
    ui->cmByte14->setEnabled(value);
    ui->cmByte15->setEnabled(value);
    ui->cmByte16->setEnabled(value);
    ui->cbByte17->setEnabled(value);

    ui->cmByte20->setEnabled(value);
    ui->cmByte21->setEnabled(value);
    ui->cmByte22->setEnabled(value);
    ui->cmByte23->setEnabled(value);
    ui->cmByte24->setEnabled(value);
    ui->cmByte25->setEnabled(value);
    ui->cmByte26->setEnabled(value);
    ui->cbByte27->setEnabled(value);

    ui->cmByte30->setEnabled(value);
    ui->cmByte31->setEnabled(value);
    ui->cmByte32->setEnabled(value);
    ui->cmByte33->setEnabled(value);
    ui->cmByte34->setEnabled(value);
    ui->cmByte35->setEnabled(value);
    ui->cmByte36->setEnabled(value);
    ui->cbByte37->setEnabled(value);

    ui->cmByte40->setEnabled(value);
    ui->cmByte41->setEnabled(value);
    ui->cmByte42->setEnabled(value);
    ui->cmByte43->setEnabled(value);
    ui->cmByte44->setEnabled(value);
    ui->cmByte45->setEnabled(value);
    ui->cmByte46->setEnabled(value);
    ui->cbByte47->setEnabled(value);

    ui->cmByte50->setEnabled(value);
    ui->cmByte51->setEnabled(value);
    ui->cmByte52->setEnabled(value);
    ui->cmByte53->setEnabled(value);
    ui->cmByte54->setEnabled(value);
    ui->cmByte55->setEnabled(value);
    ui->cmByte56->setEnabled(value);
    ui->cbByte57->setEnabled(value);

    ui->cmByte60->setEnabled(value);
    ui->cmByte61->setEnabled(value);
    ui->cmByte62->setEnabled(value);
    ui->cmByte63->setEnabled(value);
    ui->cmByte64->setEnabled(value);
    ui->cmByte65->setEnabled(value);
    ui->cmByte66->setEnabled(value);
    ui->cbByte67->setEnabled(value);

    ui->cbByte70->setEnabled(value);
    ui->cbByte71->setEnabled(value);
    ui->cbByte72->setEnabled(value);
    ui->cbByte73->setEnabled(value);
    ui->cbByte74->setEnabled(value);
    ui->cbByte75->setEnabled(value);
    ui->cbByte76->setEnabled(value);
    ui->cbByte77->setEnabled(value);

    ui->cbByte80->setEnabled(value);
    ui->cbByte81->setEnabled(value);
    ui->cbByte82->setEnabled(value);
    ui->cbByte83->setEnabled(value);
    ui->cbByte84->setEnabled(value);
    ui->cbByte85->setEnabled(value);
    ui->cbByte86->setEnabled(value);
    ui->cbByte87->setEnabled(value);

    ui->cbByte90->setEnabled(value);
    ui->cbByte91->setEnabled(value);
    ui->cbByte92->setEnabled(value);
    ui->cbByte93->setEnabled(value);
    ui->cbByte94->setEnabled(value);
    ui->cbByte95->setEnabled(value);
    ui->cbByte96->setEnabled(value);
    ui->cbByte97->setEnabled(value);

    ui->cbByte100->setEnabled(value);
    ui->cbByte101->setEnabled(value);
    ui->cbByte102->setEnabled(value);
    ui->cbByte103->setEnabled(value);
    ui->cbByte104->setEnabled(value);
    ui->cbByte105->setEnabled(value);
    ui->cbByte106->setEnabled(value);
    ui->cbByte107->setEnabled(value);

    ui->cbByte110->setEnabled(value);
    ui->cbByte111->setEnabled(value);
    ui->cbByte112->setEnabled(value);
    ui->cbByte113->setEnabled(value);
    ui->cbByte114->setEnabled(value);
    ui->cbByte115->setEnabled(value);
    ui->cbByte116->setEnabled(value);
    ui->cbByte117->setEnabled(value);
}

void MainWindow::lockToggleUI()
{
    setToggleUIEnabled(false);
}

void MainWindow::unlockToggleUI()
{
    setToggleUIEnabled(true);
}

void MainWindow::updateBytes(void) {
    for(uint8_t i = 0; i < beanFrame.getMl() - 2; i++) {
        updateByte(i);
    }
}

void MainWindow::beanFrameReceived(BeanFrame* frame) {
    LoggerFrame loggerFrame(*frame);
//    model->putFrame(frame);    
    loggerFrame.setTime(QDateTime::currentDateTime());
    loggerFrame.setTimeMsec(QDateTime::currentMSecsSinceEpoch() - logStartMSec);
    model->putLoggerFrame(&loggerFrame);
    groupModel->putLoggerFrame(&loggerFrame);
    if (ui->cbLoggerAutoScroll->isChecked()) {
        ui->tblLogger->scrollToBottom();
        ui->tblLogger->selectRow(ui->tblLogger->model()->rowCount() - 1);
    } else {
        if (selectedRowNum == -1) {
            selectedRowNum = ui->tblLogger->model()->rowCount() - 1;
            ui->tblLogger->selectRow(selectedRowNum);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    selectedRowNum = -1;
    loggerStarted = false;
    isSending = false;
    isPlaying = false;


    device = new Device();
    timer = new QTimer();
    timeoutTimer = new QTimer();
    model = new LoggerModel();
    groupModel = new LoggerGroupModel();
    toggleDbFrameModel = new ToggleDbFrameModel();
    playerModel = new PlayerModel();

    fillPriority();
    fillDestIds();
    fillMessageIds();

    updateDeviceMenu();

    ui->tbDatabase->addAction(ui->actionNewDatabase);
    ui->tbDatabase->addAction(ui->actionOpenDatabase);
    ui->tbDatabase->addAction(ui->actionSaveDatabase);

    ui->tblLogger->setModel(model);

    ui->tblLogger->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tblLogger->horizontalHeader()->setHighlightSections(false);

    ui->tblLoggerGroup->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tblLoggerGroup->horizontalHeader()->setHighlightSections(false);
    ui->tblLoggerGroup->setModel(groupModel);

    ui->tblLoggerGroup->selectRow(0);

    ui->tbToggleDbFrame->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tbToggleDbFrame->horizontalHeader()->setHighlightSections(false);
    ui->tbToggleDbFrame->setModel(toggleDbFrameModel);

    ui->tblPlayer->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tblPlayer->horizontalHeader()->setHighlightSections(false);
    ui->tblPlayer->setModel(playerModel);

    initBeanFrame();
    updateFrameBytes();
    lockToggleUI();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//#define CB_TOGGLE(a, b, c) c ? bytes[a] |= 1UL << b : bytes[a] &= ~(1UL << b);
#define CB_TOGGLE(a, b, c) c ? beanFrame.setData(a, (beanFrame.getData(a) | (1UL << b))) : beanFrame.setData(a, (beanFrame.getData(a)) & ~(1UL << b)); updateFrameBytes();

void MainWindow::on_cbByte17_toggled(bool checked)
{
    CB_TOGGLE(0, 7, checked);
    updateByte(0);
}

void MainWindow::on_cmByte16_toggled(bool checked)
{
    CB_TOGGLE(0, 6, checked);
    updateByte(0);
}

void MainWindow::on_cmByte15_toggled(bool checked)
{
    CB_TOGGLE(0, 5, checked);
    updateByte(0);
}

void MainWindow::on_cmByte14_toggled(bool checked)
{
    CB_TOGGLE(0, 4, checked);
    updateByte(0);
}

void MainWindow::on_cmByte13_toggled(bool checked)
{
    CB_TOGGLE(0, 3, checked);
    updateByte(0);
}

void MainWindow::on_cmByte12_toggled(bool checked)
{
    CB_TOGGLE(0, 2, checked);
    updateByte(0);
}

void MainWindow::on_cmByte11_toggled(bool checked)
{
    CB_TOGGLE(0, 1, checked);
    updateByte(0);
}

void MainWindow::on_cmByte10_toggled(bool checked)
{
    CB_TOGGLE(0, 0, checked);
    updateByte(0);
}

void MainWindow::on_cbByte27_toggled(bool checked)
{
    CB_TOGGLE(1, 7, checked);
    updateByte(1);
}

void MainWindow::on_cmByte26_toggled(bool checked)
{
    CB_TOGGLE(1, 6, checked);
    updateByte(1);
}

void MainWindow::on_cmByte25_toggled(bool checked)
{
    CB_TOGGLE(1, 5, checked);
    updateByte(1);
}

void MainWindow::on_cmByte24_toggled(bool checked)
{
    CB_TOGGLE(1, 4, checked);
    updateByte(1);
}

void MainWindow::on_cmByte23_toggled(bool checked)
{
    CB_TOGGLE(1, 3, checked);
    updateByte(1);
}

void MainWindow::on_cmByte22_toggled(bool checked)
{
    CB_TOGGLE(1, 2, checked);
    updateByte(1);
}

void MainWindow::on_cmByte21_toggled(bool checked)
{
    CB_TOGGLE(1, 1, checked);
    updateByte(1);
}

void MainWindow::on_cmByte20_toggled(bool checked)
{
    CB_TOGGLE(1, 0, checked);
    updateByte(1);
}

void MainWindow::on_cbByte37_toggled(bool checked)
{
    CB_TOGGLE(2, 7, checked);
    updateByte(2);
}

void MainWindow::on_cmByte36_toggled(bool checked)
{
    CB_TOGGLE(2, 6, checked);
    updateByte(2);
}

void MainWindow::on_cmByte35_toggled(bool checked)
{
    CB_TOGGLE(2, 5, checked);
    updateByte(2);
}

void MainWindow::on_cmByte34_toggled(bool checked)
{
    CB_TOGGLE(2, 4, checked);
    updateByte(2);
}

void MainWindow::on_cmByte33_toggled(bool checked)
{
    CB_TOGGLE(2, 3, checked);
    updateByte(2);
}

void MainWindow::on_cmByte32_toggled(bool checked)
{
    CB_TOGGLE(2, 2, checked);
    updateByte(2);
}

void MainWindow::on_cmByte31_toggled(bool checked)
{
    CB_TOGGLE(2, 1, checked);
    updateByte(2);
}

void MainWindow::on_cmByte30_toggled(bool checked)
{
    CB_TOGGLE(2, 0, checked);
    updateByte(2);
}

void MainWindow::on_cbByte47_toggled(bool checked)
{
    CB_TOGGLE(3, 7, checked);
    updateByte(3);
}

void MainWindow::on_cmByte46_toggled(bool checked)
{
    CB_TOGGLE(3, 6, checked);
    updateByte(3);
}

void MainWindow::on_cmByte45_toggled(bool checked)
{
    CB_TOGGLE(3, 5, checked);
    updateByte(3);
}

void MainWindow::on_cmByte44_toggled(bool checked)
{
    CB_TOGGLE(3, 4, checked);
    updateByte(3);
}

void MainWindow::on_cmByte43_toggled(bool checked)
{
    CB_TOGGLE(3, 3, checked);
    updateByte(3);
}

void MainWindow::on_cmByte42_toggled(bool checked)
{
    CB_TOGGLE(3, 2, checked);
    updateByte(3);
}

void MainWindow::on_cmByte41_toggled(bool checked)
{
    CB_TOGGLE(3, 1, checked);
    updateByte(3);
}

void MainWindow::on_cmByte40_toggled(bool checked)
{
    CB_TOGGLE(3, 0, checked);
    updateByte(3);
}

void MainWindow::on_cbByte57_toggled(bool checked)
{
    CB_TOGGLE(4, 7, checked);
    updateByte(4);
}

void MainWindow::on_cmByte56_toggled(bool checked)
{
    CB_TOGGLE(4, 6, checked);
    updateByte(4);
}

void MainWindow::on_cmByte55_toggled(bool checked)
{
    CB_TOGGLE(4, 5, checked);
    updateByte(4);
}

void MainWindow::on_cmByte54_toggled(bool checked)
{
    CB_TOGGLE(4, 4, checked);
    updateByte(4);
}

void MainWindow::on_cmByte53_toggled(bool checked)
{
    CB_TOGGLE(4, 3, checked);
    updateByte(4);
}

void MainWindow::on_cmByte52_toggled(bool checked)
{
    CB_TOGGLE(4, 2, checked);
    updateByte(4);
}

void MainWindow::on_cmByte51_toggled(bool checked)
{
    CB_TOGGLE(4, 1, checked);
    updateByte(4);
}

void MainWindow::on_cmByte50_toggled(bool checked)
{
    CB_TOGGLE(4, 0, checked);
    updateByte(4);
}

void MainWindow::on_cbByte67_toggled(bool checked)
{
    CB_TOGGLE(5, 7, checked);
    updateByte(5);
}

void MainWindow::on_cmByte66_toggled(bool checked)
{
    CB_TOGGLE(5, 6, checked);
    updateByte(5);
}

void MainWindow::on_cmByte65_toggled(bool checked)
{
    CB_TOGGLE(5, 5, checked);
    updateByte(5);
}

void MainWindow::on_cmByte64_toggled(bool checked)
{
    CB_TOGGLE(5, 4, checked);
    updateByte(5);
}

void MainWindow::on_cmByte63_toggled(bool checked)
{
    CB_TOGGLE(5, 3, checked);
    updateByte(5);
}

void MainWindow::on_cmByte62_toggled(bool checked)
{
    CB_TOGGLE(5, 2, checked);
    updateByte(5);
}

void MainWindow::on_cmByte61_toggled(bool checked)
{
    CB_TOGGLE(5, 1, checked);
    updateByte(5);
}

void MainWindow::on_cmByte60_toggled(bool checked)
{
    CB_TOGGLE(5, 0, checked);
    updateByte(5);
}

void MainWindow::on_btnClearBytes_clicked()
{

    ui->cbByte17->setChecked(false);
    ui->cmByte16->setChecked(false);
    ui->cmByte15->setChecked(false);
    ui->cmByte14->setChecked(false);
    ui->cmByte13->setChecked(false);
    ui->cmByte12->setChecked(false);
    ui->cmByte11->setChecked(false);
    ui->cmByte10->setChecked(false);

    ui->cbByte27->setChecked(false);
    ui->cmByte26->setChecked(false);
    ui->cmByte25->setChecked(false);
    ui->cmByte24->setChecked(false);
    ui->cmByte23->setChecked(false);
    ui->cmByte22->setChecked(false);
    ui->cmByte21->setChecked(false);
    ui->cmByte20->setChecked(false);

    ui->cbByte37->setChecked(false);
    ui->cmByte36->setChecked(false);
    ui->cmByte35->setChecked(false);
    ui->cmByte34->setChecked(false);
    ui->cmByte33->setChecked(false);
    ui->cmByte32->setChecked(false);
    ui->cmByte31->setChecked(false);
    ui->cmByte30->setChecked(false);

    ui->cbByte47->setChecked(false);
    ui->cmByte46->setChecked(false);
    ui->cmByte45->setChecked(false);
    ui->cmByte44->setChecked(false);
    ui->cmByte43->setChecked(false);
    ui->cmByte42->setChecked(false);
    ui->cmByte41->setChecked(false);
    ui->cmByte40->setChecked(false);

    ui->cbByte57->setChecked(false);
    ui->cmByte56->setChecked(false);
    ui->cmByte55->setChecked(false);
    ui->cmByte54->setChecked(false);
    ui->cmByte53->setChecked(false);
    ui->cmByte52->setChecked(false);
    ui->cmByte51->setChecked(false);
    ui->cmByte50->setChecked(false);

    ui->cbByte67->setChecked(false);
    ui->cmByte66->setChecked(false);
    ui->cmByte65->setChecked(false);
    ui->cmByte64->setChecked(false);
    ui->cmByte63->setChecked(false);
    ui->cmByte62->setChecked(false);
    ui->cmByte61->setChecked(false);
    ui->cmByte60->setChecked(false);

    ui->cbByte77->setChecked(false);
    ui->cbByte76->setChecked(false);
    ui->cbByte75->setChecked(false);
    ui->cbByte74->setChecked(false);
    ui->cbByte73->setChecked(false);
    ui->cbByte72->setChecked(false);
    ui->cbByte71->setChecked(false);
    ui->cbByte70->setChecked(false);

    ui->cbByte77->setChecked(false);
    ui->cbByte76->setChecked(false);
    ui->cbByte75->setChecked(false);
    ui->cbByte74->setChecked(false);
    ui->cbByte73->setChecked(false);
    ui->cbByte72->setChecked(false);
    ui->cbByte71->setChecked(false);
    ui->cbByte70->setChecked(false);

    ui->cbByte87->setChecked(false);
    ui->cbByte86->setChecked(false);
    ui->cbByte85->setChecked(false);
    ui->cbByte84->setChecked(false);
    ui->cbByte83->setChecked(false);
    ui->cbByte82->setChecked(false);
    ui->cbByte81->setChecked(false);
    ui->cbByte80->setChecked(false);

    ui->cbByte97->setChecked(false);
    ui->cbByte96->setChecked(false);
    ui->cbByte95->setChecked(false);
    ui->cbByte94->setChecked(false);
    ui->cbByte93->setChecked(false);
    ui->cbByte92->setChecked(false);
    ui->cbByte91->setChecked(false);
    ui->cbByte90->setChecked(false);

    ui->cbByte107->setChecked(false);
    ui->cbByte106->setChecked(false);
    ui->cbByte105->setChecked(false);
    ui->cbByte104->setChecked(false);
    ui->cbByte103->setChecked(false);
    ui->cbByte102->setChecked(false);
    ui->cbByte101->setChecked(false);
    ui->cbByte100->setChecked(false);

    ui->cbByte117->setChecked(false);
    ui->cbByte116->setChecked(false);
    ui->cbByte115->setChecked(false);
    ui->cbByte114->setChecked(false);
    ui->cbByte113->setChecked(false);
    ui->cbByte112->setChecked(false);
    ui->cbByte111->setChecked(false);
    ui->cbByte110->setChecked(false);

    updateFrameBytes();

}

void MainWindow::serialReceived()
{
//    QByteArray ba;
//    ba= ser->readAll();
//    qInfo(ba);

}

void MainWindow::timerAlarm()
{
    if (device->isConnected()) {
//        deviceSend();
    }
}

void MainWindow::on_cbMessageId_currentIndexChanged(int index)
{
    updateFrameBytes();
    if (device->isConnected()) {
        if (isLiveSend) {
//            deviceSend();
        }
    }
}

void MainWindow::on_cbMessageId_currentTextChanged(const QString &arg1)
{
    if (device->isConnected()) {
        if (isLiveSend) {
//            deviceSend();
        }
    }
}

void MainWindow::on_btnSetBytes_clicked()
{
    ui->cbByte17->setChecked(true);
    ui->cmByte16->setChecked(true);
    ui->cmByte15->setChecked(true);
    ui->cmByte14->setChecked(true);
    ui->cmByte13->setChecked(true);
    ui->cmByte12->setChecked(true);
    ui->cmByte11->setChecked(true);
    ui->cmByte10->setChecked(true);

    ui->cbByte27->setChecked(true);
    ui->cmByte26->setChecked(true);
    ui->cmByte25->setChecked(true);
    ui->cmByte24->setChecked(true);
    ui->cmByte23->setChecked(true);
    ui->cmByte22->setChecked(true);
    ui->cmByte21->setChecked(true);
    ui->cmByte20->setChecked(true);

    ui->cbByte37->setChecked(true);
    ui->cmByte36->setChecked(true);
    ui->cmByte35->setChecked(true);
    ui->cmByte34->setChecked(true);
    ui->cmByte33->setChecked(true);
    ui->cmByte32->setChecked(true);
    ui->cmByte31->setChecked(true);
    ui->cmByte30->setChecked(true);

    ui->cbByte47->setChecked(true);
    ui->cmByte46->setChecked(true);
    ui->cmByte45->setChecked(true);
    ui->cmByte44->setChecked(true);
    ui->cmByte43->setChecked(true);
    ui->cmByte42->setChecked(true);
    ui->cmByte41->setChecked(true);
    ui->cmByte40->setChecked(true);

    ui->cbByte57->setChecked(true);
    ui->cmByte56->setChecked(true);
    ui->cmByte55->setChecked(true);
    ui->cmByte54->setChecked(true);
    ui->cmByte53->setChecked(true);
    ui->cmByte52->setChecked(true);
    ui->cmByte51->setChecked(true);
    ui->cmByte50->setChecked(true);

    ui->cbByte67->setChecked(true);
    ui->cmByte66->setChecked(true);
    ui->cmByte65->setChecked(true);
    ui->cmByte64->setChecked(true);
    ui->cmByte63->setChecked(true);
    ui->cmByte62->setChecked(true);
    ui->cmByte61->setChecked(true);
    ui->cmByte60->setChecked(true);

    ui->cbByte67->setChecked(true);
    ui->cmByte66->setChecked(true);
    ui->cmByte65->setChecked(true);
    ui->cmByte64->setChecked(true);
    ui->cmByte63->setChecked(true);
    ui->cmByte62->setChecked(true);
    ui->cmByte61->setChecked(true);
    ui->cmByte60->setChecked(true);

    ui->cbByte77->setChecked(true);
    ui->cbByte76->setChecked(true);
    ui->cbByte75->setChecked(true);
    ui->cbByte74->setChecked(true);
    ui->cbByte73->setChecked(true);
    ui->cbByte72->setChecked(true);
    ui->cbByte71->setChecked(true);
    ui->cbByte70->setChecked(true);

    ui->cbByte87->setChecked(true);
    ui->cbByte86->setChecked(true);
    ui->cbByte85->setChecked(true);
    ui->cbByte84->setChecked(true);
    ui->cbByte83->setChecked(true);
    ui->cbByte82->setChecked(true);
    ui->cbByte81->setChecked(true);
    ui->cbByte80->setChecked(true);

    ui->cbByte97->setChecked(true);
    ui->cbByte96->setChecked(true);
    ui->cbByte95->setChecked(true);
    ui->cbByte94->setChecked(true);
    ui->cbByte93->setChecked(true);
    ui->cbByte92->setChecked(true);
    ui->cbByte91->setChecked(true);
    ui->cbByte90->setChecked(true);

    ui->cbByte107->setChecked(true);
    ui->cbByte106->setChecked(true);
    ui->cbByte105->setChecked(true);
    ui->cbByte104->setChecked(true);
    ui->cbByte103->setChecked(true);
    ui->cbByte102->setChecked(true);
    ui->cbByte101->setChecked(true);
    ui->cbByte100->setChecked(true);

    ui->cbByte117->setChecked(true);
    ui->cbByte116->setChecked(true);
    ui->cbByte115->setChecked(true);
    ui->cbByte114->setChecked(true);
    ui->cbByte113->setChecked(true);
    ui->cbByte112->setChecked(true);
    ui->cbByte111->setChecked(true);
    ui->cbByte110->setChecked(true);

    updateFrameBytes();
}

void MainWindow::on_btnSetBits1_clicked()
{
    ui->cbByte17->setChecked(true);
    ui->cmByte16->setChecked(true);
    ui->cmByte15->setChecked(true);
    ui->cmByte14->setChecked(true);
    ui->cmByte13->setChecked(true);
    ui->cmByte12->setChecked(true);
    ui->cmByte11->setChecked(true);
    ui->cmByte10->setChecked(true);
}

void MainWindow::on_btnSetBits2_clicked()
{
    ui->cbByte27->setChecked(true);
    ui->cmByte26->setChecked(true);
    ui->cmByte25->setChecked(true);
    ui->cmByte24->setChecked(true);
    ui->cmByte23->setChecked(true);
    ui->cmByte22->setChecked(true);
    ui->cmByte21->setChecked(true);
    ui->cmByte20->setChecked(true);
}

void MainWindow::on_btnSetBits3_clicked()
{
    ui->cbByte37->setChecked(true);
    ui->cmByte36->setChecked(true);
    ui->cmByte35->setChecked(true);
    ui->cmByte34->setChecked(true);
    ui->cmByte33->setChecked(true);
    ui->cmByte32->setChecked(true);
    ui->cmByte31->setChecked(true);
    ui->cmByte30->setChecked(true);

}

void MainWindow::on_btnSetBits4_clicked()
{
    ui->cbByte47->setChecked(true);
    ui->cmByte46->setChecked(true);
    ui->cmByte45->setChecked(true);
    ui->cmByte44->setChecked(true);
    ui->cmByte43->setChecked(true);
    ui->cmByte42->setChecked(true);
    ui->cmByte41->setChecked(true);
    ui->cmByte40->setChecked(true);
}

void MainWindow::on_btnSetBits5_clicked()
{
    ui->cbByte57->setChecked(true);
    ui->cmByte56->setChecked(true);
    ui->cmByte55->setChecked(true);
    ui->cmByte54->setChecked(true);
    ui->cmByte53->setChecked(true);
    ui->cmByte52->setChecked(true);
    ui->cmByte51->setChecked(true);
    ui->cmByte50->setChecked(true);
}

void MainWindow::on_btnSetBits6_clicked()
{
    ui->cbByte67->setChecked(true);
    ui->cmByte66->setChecked(true);
    ui->cmByte65->setChecked(true);
    ui->cmByte64->setChecked(true);
    ui->cmByte63->setChecked(true);
    ui->cmByte62->setChecked(true);
    ui->cmByte61->setChecked(true);
    ui->cmByte60->setChecked(true);
}

void MainWindow::on_btnClearBits1_clicked()
{
    ui->cbByte17->setChecked(false);
    ui->cmByte16->setChecked(false);
    ui->cmByte15->setChecked(false);
    ui->cmByte14->setChecked(false);
    ui->cmByte13->setChecked(false);
    ui->cmByte12->setChecked(false);
    ui->cmByte11->setChecked(false);
    ui->cmByte10->setChecked(false);
}

void MainWindow::on_btnClearBits2_clicked()
{
    ui->cbByte27->setChecked(false);
    ui->cmByte26->setChecked(false);
    ui->cmByte25->setChecked(false);
    ui->cmByte24->setChecked(false);
    ui->cmByte23->setChecked(false);
    ui->cmByte22->setChecked(false);
    ui->cmByte21->setChecked(false);
    ui->cmByte20->setChecked(false);
}

void MainWindow::on_btnClearBits3_clicked()
{
    ui->cbByte37->setChecked(false);
    ui->cmByte36->setChecked(false);
    ui->cmByte35->setChecked(false);
    ui->cmByte34->setChecked(false);
    ui->cmByte33->setChecked(false);
    ui->cmByte32->setChecked(false);
    ui->cmByte31->setChecked(false);
    ui->cmByte30->setChecked(false);
}

void MainWindow::on_btnClearBits4_clicked()
{
    ui->cbByte47->setChecked(false);
    ui->cmByte46->setChecked(false);
    ui->cmByte45->setChecked(false);
    ui->cmByte44->setChecked(false);
    ui->cmByte43->setChecked(false);
    ui->cmByte42->setChecked(false);
    ui->cmByte41->setChecked(false);
    ui->cmByte40->setChecked(false);
}

void MainWindow::on_btnClearBits5_clicked()
{
    ui->cbByte57->setChecked(false);
    ui->cmByte56->setChecked(false);
    ui->cmByte55->setChecked(false);
    ui->cmByte54->setChecked(false);
    ui->cmByte53->setChecked(false);
    ui->cmByte52->setChecked(false);
    ui->cmByte51->setChecked(false);
    ui->cmByte50->setChecked(false);
}

void MainWindow::on_btnClearBits6_clicked()
{
    ui->cbByte67->setChecked(false);
    ui->cmByte66->setChecked(false);
    ui->cmByte65->setChecked(false);
    ui->cmByte64->setChecked(false);
    ui->cmByte63->setChecked(false);
    ui->cmByte62->setChecked(false);
    ui->cmByte61->setChecked(false);
    ui->cmByte60->setChecked(false);
}

void MainWindow::on_cbDestId_currentTextChanged(const QString &arg1)
{
    if (device->isConnected()) {
        if (isLiveSend) {
//            deviceSend();
        }
    }
}

void MainWindow::on_cbPriority_currentTextChanged(const QString &arg1)
{
    if (device->isConnected()) {
        if (isLiveSend) {
//            deviceSend();
        }
    }
}

void MainWindow::on_btnSendBytes_clicked()
{
    if (device->isConnected()) {
        if (isSending) {
            return;
        }
        QObject::connect(timeoutTimer, &QTimer::timeout, this, &MainWindow::onTimeout);
        QObject::connect(device, &Device::beanFrameTransmitted, this, &MainWindow::beanFrameTransmitted);
        QObject::connect(device, &Device::beanFrameTransmitError, this, &MainWindow::beanFrameTransmitError);
        timeoutTimer->setInterval(500);
        timeoutTimer->setSingleShot(true);
        timeoutTimer->start();
        deviceSend(&beanFrame);
    } else {
        QMessageBox::warning(this, "Send error", "Device is not connected");
    }
}

void MainWindow::on_cbByte77_toggled(bool checked)
{
    CB_TOGGLE(6, 7, checked);
    updateByte(6);
}

void MainWindow::on_cbByte76_toggled(bool checked)
{
    CB_TOGGLE(6, 6, checked);
    updateByte(6);
}

void MainWindow::on_cbByte75_toggled(bool checked)
{
    CB_TOGGLE(6, 5, checked);
    updateByte(6);
}

void MainWindow::on_cbByte74_toggled(bool checked)
{
    CB_TOGGLE(6, 4, checked);
    updateByte(6);
}

void MainWindow::on_cbByte73_toggled(bool checked)
{
    CB_TOGGLE(6, 3, checked);
    updateByte(6);
}

void MainWindow::on_cbByte72_toggled(bool checked)
{
    CB_TOGGLE(6, 2, checked);
    updateByte(6);
}

void MainWindow::on_cbByte71_toggled(bool checked)
{
    CB_TOGGLE(6, 1, checked);
    updateByte(6);
}

void MainWindow::on_cbByte70_toggled(bool checked)
{
    CB_TOGGLE(6, 0, checked);
    updateByte(6);
}

void MainWindow::on_cbByte87_toggled(bool checked)
{
    CB_TOGGLE(7, 7, checked);
    updateByte(7);
}

void MainWindow::on_cbByte86_toggled(bool checked)
{
    CB_TOGGLE(7, 6, checked);
    updateByte(7);
}

void MainWindow::on_cbByte85_toggled(bool checked)
{
    CB_TOGGLE(7, 5, checked);
    updateByte(7);
}

void MainWindow::on_cbByte84_toggled(bool checked)
{
    CB_TOGGLE(7, 4, checked);
    updateByte(7);
}

void MainWindow::on_cbByte83_toggled(bool checked)
{
    CB_TOGGLE(7, 3, checked);
    updateByte(7);
}

void MainWindow::on_cbByte82_toggled(bool checked)
{
    CB_TOGGLE(7, 2, checked);
    updateByte(7);
}

void MainWindow::on_cbByte81_toggled(bool checked)
{
    CB_TOGGLE(7, 1, checked);
    updateByte(7);
}

void MainWindow::on_cbByte80_toggled(bool checked)
{
    CB_TOGGLE(7, 0, checked);
    updateByte(7);
}

void MainWindow::on_cbByte97_toggled(bool checked)
{
    CB_TOGGLE(8, 7, checked);
    updateByte(8);
}

void MainWindow::on_cbByte96_toggled(bool checked)
{
    CB_TOGGLE(8, 6, checked);
    updateByte(8);
}

void MainWindow::on_cbByte95_toggled(bool checked)
{
    CB_TOGGLE(8, 5, checked);
    updateByte(8);
}

void MainWindow::on_cbByte94_toggled(bool checked)
{
    CB_TOGGLE(8, 4, checked);
    updateByte(8);
}

void MainWindow::on_cbByte93_toggled(bool checked)
{
    CB_TOGGLE(8, 3, checked);
    updateByte(8);
}

void MainWindow::on_cbByte92_toggled(bool checked)
{
    CB_TOGGLE(8, 2, checked);
    updateByte(8);
}

void MainWindow::on_cbByte91_toggled(bool checked)
{
    CB_TOGGLE(8, 1, checked);
    updateByte(8);
}

void MainWindow::on_cbByte90_toggled(bool checked)
{
    CB_TOGGLE(8, 0, checked);
    updateByte(8);
}

void MainWindow::on_cbByte107_toggled(bool checked)
{
    CB_TOGGLE(9, 7, checked);
    updateByte(9);
}

void MainWindow::on_cbByte106_toggled(bool checked)
{
    CB_TOGGLE(9, 6, checked);
    updateByte(9);
}

void MainWindow::on_cbByte105_toggled(bool checked)
{
    CB_TOGGLE(9, 5, checked);
    updateByte(9);
}

void MainWindow::on_cbByte104_toggled(bool checked)
{
    CB_TOGGLE(9, 4, checked);
    updateByte(9);
}

void MainWindow::on_cbByte103_toggled(bool checked)
{
    CB_TOGGLE(9, 3, checked);
    updateByte(9);
}

void MainWindow::on_cbByte102_toggled(bool checked)
{
    CB_TOGGLE(9, 2, checked);
    updateByte(9);
}

void MainWindow::on_cbByte101_toggled(bool checked)
{
    CB_TOGGLE(9, 1, checked);
    updateByte(9);
}

void MainWindow::on_cbByte100_toggled(bool checked)
{
    CB_TOGGLE(9, 0, checked);
    updateByte(9);
}

void MainWindow::on_cbByte117_toggled(bool checked)
{
    CB_TOGGLE(10, 7, checked);
    updateByte(10);
}

void MainWindow::on_cbByte116_toggled(bool checked)
{
    CB_TOGGLE(10, 6, checked);
    updateByte(10);
}

void MainWindow::on_cbByte115_toggled(bool checked)
{
    CB_TOGGLE(10, 5, checked);
    updateByte(10);
}

void MainWindow::on_cbByte114_toggled(bool checked)
{
    CB_TOGGLE(10, 4, checked);
    updateByte(10);
}

void MainWindow::on_cbByte113_toggled(bool checked)
{
    CB_TOGGLE(10, 3, checked);
    updateByte(10);
}

void MainWindow::on_cbByte112_toggled(bool checked)
{
    CB_TOGGLE(10, 2, checked);
    updateByte(10);
}

void MainWindow::on_cbByte111_toggled(bool checked)
{
    CB_TOGGLE(10, 1, checked);
    updateByte(10);
}

void MainWindow::on_cbByte110_toggled(bool checked)
{
    CB_TOGGLE(10, 0, checked);
    updateByte(10);
}

void MainWindow::on_btnSetBits7_clicked()
{
    ui->cbByte77->setChecked(true);
    ui->cbByte76->setChecked(true);
    ui->cbByte75->setChecked(true);
    ui->cbByte74->setChecked(true);
    ui->cbByte73->setChecked(true);
    ui->cbByte72->setChecked(true);
    ui->cbByte71->setChecked(true);
    ui->cbByte70->setChecked(true);
}

void MainWindow::on_btnSetBits8_clicked()
{
    ui->cbByte87->setChecked(true);
    ui->cbByte86->setChecked(true);
    ui->cbByte85->setChecked(true);
    ui->cbByte84->setChecked(true);
    ui->cbByte83->setChecked(true);
    ui->cbByte82->setChecked(true);
    ui->cbByte81->setChecked(true);
    ui->cbByte80->setChecked(true);
}

void MainWindow::on_btnSetBits9_clicked()
{
    ui->cbByte97->setChecked(true);
    ui->cbByte96->setChecked(true);
    ui->cbByte95->setChecked(true);
    ui->cbByte94->setChecked(true);
    ui->cbByte93->setChecked(true);
    ui->cbByte92->setChecked(true);
    ui->cbByte91->setChecked(true);
    ui->cbByte90->setChecked(true);
}

void MainWindow::on_btnSetBits10_clicked()
{
    ui->cbByte107->setChecked(true);
    ui->cbByte106->setChecked(true);
    ui->cbByte105->setChecked(true);
    ui->cbByte104->setChecked(true);
    ui->cbByte103->setChecked(true);
    ui->cbByte102->setChecked(true);
    ui->cbByte101->setChecked(true);
    ui->cbByte100->setChecked(true);
}

void MainWindow::on_btnSetBits11_clicked()
{
    ui->cbByte117->setChecked(true);
    ui->cbByte116->setChecked(true);
    ui->cbByte115->setChecked(true);
    ui->cbByte114->setChecked(true);
    ui->cbByte113->setChecked(true);
    ui->cbByte112->setChecked(true);
    ui->cbByte111->setChecked(true);
    ui->cbByte110->setChecked(true);
}

void MainWindow::on_btnClearBits7_clicked()
{
    ui->cbByte77->setChecked(false);
    ui->cbByte76->setChecked(false);
    ui->cbByte75->setChecked(false);
    ui->cbByte74->setChecked(false);
    ui->cbByte73->setChecked(false);
    ui->cbByte72->setChecked(false);
    ui->cbByte71->setChecked(false);
    ui->cbByte70->setChecked(false);
}

void MainWindow::on_btnClearBits8_clicked()
{
    ui->cbByte87->setChecked(false);
    ui->cbByte86->setChecked(false);
    ui->cbByte85->setChecked(false);
    ui->cbByte84->setChecked(false);
    ui->cbByte83->setChecked(false);
    ui->cbByte82->setChecked(false);
    ui->cbByte81->setChecked(false);
    ui->cbByte80->setChecked(false);
}

void MainWindow::on_btnClearBits9_clicked()
{
    ui->cbByte97->setChecked(false);
    ui->cbByte96->setChecked(false);
    ui->cbByte95->setChecked(false);
    ui->cbByte94->setChecked(false);
    ui->cbByte93->setChecked(false);
    ui->cbByte92->setChecked(false);
    ui->cbByte91->setChecked(false);
    ui->cbByte90->setChecked(false);
}

void MainWindow::on_btnClearBits10_clicked()
{
    ui->cbByte107->setChecked(false);
    ui->cbByte106->setChecked(false);
    ui->cbByte105->setChecked(false);
    ui->cbByte104->setChecked(false);
    ui->cbByte103->setChecked(false);
    ui->cbByte102->setChecked(false);
    ui->cbByte101->setChecked(false);
    ui->cbByte100->setChecked(false);
}

void MainWindow::on_btnClearBits11_clicked()
{
    ui->cbByte117->setChecked(false);
    ui->cbByte116->setChecked(false);
    ui->cbByte115->setChecked(false);
    ui->cbByte114->setChecked(false);
    ui->cbByte113->setChecked(false);
    ui->cbByte112->setChecked(false);
    ui->cbByte111->setChecked(false);
    ui->cbByte110->setChecked(false);
}

void MainWindow::on_sbBytesCount_valueChanged(int arg1)
{
    updateFrameBytes();
}

void MainWindow::on_cbPriority_currentIndexChanged(int index)
{
    updateFrameBytes();
}

void MainWindow::on_cbDestId_currentIndexChanged(int index)
{
    updateFrameBytes();
}

void MainWindow::on_actionConnect_triggered()
{
    DeviceConnectDialog* w = new DeviceConnectDialog(this);
    w->setModal(true);
    if (w->exec() == QDialog::Accepted) {
        if (w->getSerialPortName().isNull()) {
            QMessageBox::warning(this, "Serial port error", "Could not open serial port");
            return;
        }
        device->connect(w->getSerialPortName());
        if (!device->isConnected()) {
            QMessageBox::warning(this, "Serial port error", "Could not open serial port. May be in use?");
        }
        updateDeviceMenu();
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    device->disconnect();
    updateDeviceMenu();
}

void MainWindow::on_btnLoggerClear_clicked()
{
    model->clear();
    groupModel->clear();
    selectedRowNum = -1;
}

void MainWindow::on_btnLoggerStart_clicked()
{

    if (!device->isConnected()) {
        QMessageBox::warning(this, "Logger start error", "Device is not connected");
        return;
    }

    if (!loggerStarted) {

        LoggerCaptureSetup* w = new LoggerCaptureSetup(this);
        w->setModal(true);

        if (w->exec() != QDialog::Accepted) {
            return;
        }

        logStartMSec = QDateTime::currentMSecsSinceEpoch();
        QObject::connect(device, &Device::beanFrameReceived, this, &MainWindow::beanFrameReceived);
        ui->btnLoggerStart->setText("Stop");
    } else {
        QObject::disconnect(device, &Device::beanFrameReceived, this, &MainWindow::beanFrameReceived);
        ui->btnLoggerStart->setText("Start");
    }

    loggerStarted = !loggerStarted;
}

void MainWindow::on_tblLogger_doubleClicked(const QModelIndex &index)
{
    LoggerFrameDialog* w = new LoggerFrameDialog(this);
    w->setModal(true);
    if (w->exec() == QDialog::Accepted) {
    }
}

void MainWindow::LogFilterApply()
{
    BeanFrameGroup* fg = groupModel->getFrameGroup(ui->tblLoggerGroup->currentIndex().row());
    if (ui->cmLogFilter->currentIndex() == 0) {
        model->setFilter(fg->getMsgId(), fg->getDstId());
    } else if (ui->cmLogFilter->currentIndex() == 1) {
        model->setFilter(-1, fg->getDstId());
    } else if (ui->cmLogFilter->currentIndex() == 2) {
        model->setFilter(fg->getMsgId(), -1);
    }
    ui->tblLogger->selectRow(0);
}

void MainWindow::on_tblLoggerGroup_clicked(const QModelIndex &index)
{
    LogFilterApply();
}

void MainWindow::on_cmLogFilter_currentIndexChanged(int index)
{
    LogFilterApply();
}

void MainWindow::on_tbDatabase_clicked()
{
    ui->tbDatabase->showMenu();
}

void MainWindow::initBeanFrame()
{
    beanFrame.setDstId(0xFE);
    beanFrame.setMsgId(0x83);
    beanFrame.setPri(1);
    beanFrame.setMl(13);
    for(uint8_t i = 0; i < beanFrame.getMl() - 2; i++) {
        beanFrame.setData(i, 0);
    }
    beanFrame.setAck(0);
}

void MainWindow::on_btnAddFrame_clicked()
{
    initBeanFrame();
    toggleDbFrameModel->addFrame(beanFrame);
    ui->tbToggleDbFrame->selectRow(toggleDbFrameModel->rowCount() - 1);
    toggleDbFrameModel->updateFrame(ui->tbToggleDbFrame->currentIndex(), beanFrame);
    loadDbFrame();
    unlockToggleUI();
}

void MainWindow::on_btnDeleteFrame_clicked()
{
    int row = ui->tbToggleDbFrame->currentIndex().row();
    toggleDbFrameModel->deleteFrame(ui->tbToggleDbFrame->currentIndex());
    if (row > toggleDbFrameModel->rowCount() - 1) {
        row = toggleDbFrameModel->rowCount() - 1;
    }
    ui->tbToggleDbFrame->selectRow(row);
    if (toggleDbFrameModel->rowCount() > 0) {
        beanFrame = toggleDbFrameModel->getFrame(ui->tbToggleDbFrame->currentIndex().row());
        loadDbFrame();
    } else {
        initBeanFrame();
        loadDbFrame();
        lockToggleUI();
    }
}

void MainWindow::on_actionNewDatabase_triggered()
{
    toggleDbFrameModel->clear();
    initBeanFrame();
    loadDbFrame();
    lockToggleUI();
}

void MainWindow::loadDbFrame()
{

    ui->cbMessageId->blockSignals(true);
    ui->cbDestId->blockSignals(true);
    ui->cbPriority->blockSignals(true);
    ui->sbBytesCount->blockSignals(true);

    ui->cmByte10->blockSignals(true);
    ui->cmByte11->blockSignals(true);
    ui->cmByte12->blockSignals(true);
    ui->cmByte13->blockSignals(true);
    ui->cmByte14->blockSignals(true);
    ui->cmByte15->blockSignals(true);
    ui->cmByte16->blockSignals(true);
    ui->cbByte17->blockSignals(true);

    ui->cmByte20->blockSignals(true);
    ui->cmByte21->blockSignals(true);
    ui->cmByte22->blockSignals(true);
    ui->cmByte23->blockSignals(true);
    ui->cmByte24->blockSignals(true);
    ui->cmByte25->blockSignals(true);
    ui->cmByte26->blockSignals(true);
    ui->cbByte27->blockSignals(true);

    ui->cmByte30->blockSignals(true);
    ui->cmByte31->blockSignals(true);
    ui->cmByte32->blockSignals(true);
    ui->cmByte33->blockSignals(true);
    ui->cmByte34->blockSignals(true);
    ui->cmByte35->blockSignals(true);
    ui->cmByte36->blockSignals(true);
    ui->cbByte37->blockSignals(true);

    ui->cmByte40->blockSignals(true);
    ui->cmByte41->blockSignals(true);
    ui->cmByte42->blockSignals(true);
    ui->cmByte43->blockSignals(true);
    ui->cmByte44->blockSignals(true);
    ui->cmByte45->blockSignals(true);
    ui->cmByte46->blockSignals(true);
    ui->cbByte47->blockSignals(true);

    ui->cmByte50->blockSignals(true);
    ui->cmByte51->blockSignals(true);
    ui->cmByte52->blockSignals(true);
    ui->cmByte53->blockSignals(true);
    ui->cmByte54->blockSignals(true);
    ui->cmByte55->blockSignals(true);
    ui->cmByte56->blockSignals(true);
    ui->cbByte57->blockSignals(true);

    ui->cmByte60->blockSignals(true);
    ui->cmByte61->blockSignals(true);
    ui->cmByte62->blockSignals(true);
    ui->cmByte63->blockSignals(true);
    ui->cmByte64->blockSignals(true);
    ui->cmByte65->blockSignals(true);
    ui->cmByte66->blockSignals(true);
    ui->cbByte67->blockSignals(true);

    ui->cbByte70->blockSignals(true);
    ui->cbByte71->blockSignals(true);
    ui->cbByte72->blockSignals(true);
    ui->cbByte73->blockSignals(true);
    ui->cbByte74->blockSignals(true);
    ui->cbByte75->blockSignals(true);
    ui->cbByte76->blockSignals(true);
    ui->cbByte77->blockSignals(true);

    ui->cbByte80->blockSignals(true);
    ui->cbByte81->blockSignals(true);
    ui->cbByte82->blockSignals(true);
    ui->cbByte83->blockSignals(true);
    ui->cbByte84->blockSignals(true);
    ui->cbByte85->blockSignals(true);
    ui->cbByte86->blockSignals(true);
    ui->cbByte87->blockSignals(true);

    ui->cbByte90->blockSignals(true);
    ui->cbByte91->blockSignals(true);
    ui->cbByte92->blockSignals(true);
    ui->cbByte93->blockSignals(true);
    ui->cbByte94->blockSignals(true);
    ui->cbByte95->blockSignals(true);
    ui->cbByte96->blockSignals(true);
    ui->cbByte97->blockSignals(true);

    ui->cbByte100->blockSignals(true);
    ui->cbByte101->blockSignals(true);
    ui->cbByte102->blockSignals(true);
    ui->cbByte103->blockSignals(true);
    ui->cbByte104->blockSignals(true);
    ui->cbByte105->blockSignals(true);
    ui->cbByte106->blockSignals(true);
    ui->cbByte107->blockSignals(true);

    ui->cbByte110->blockSignals(true);
    ui->cbByte111->blockSignals(true);
    ui->cbByte112->blockSignals(true);
    ui->cbByte113->blockSignals(true);
    ui->cbByte114->blockSignals(true);
    ui->cbByte115->blockSignals(true);
    ui->cbByte116->blockSignals(true);
    ui->cbByte117->blockSignals(true);

    ui->cmByte10->setChecked(beanFrame.getData(0) & (1 << 0));
    ui->cmByte11->setChecked(beanFrame.getData(0) & (1 << 1));
    ui->cmByte12->setChecked(beanFrame.getData(0) & (1 << 2));
    ui->cmByte13->setChecked(beanFrame.getData(0) & (1 << 3));
    ui->cmByte14->setChecked(beanFrame.getData(0) & (1 << 4));
    ui->cmByte15->setChecked(beanFrame.getData(0) & (1 << 5));
    ui->cmByte16->setChecked(beanFrame.getData(0) & (1 << 6));
    ui->cbByte17->setChecked(beanFrame.getData(0) & (1 << 7));

    ui->cmByte20->setChecked(beanFrame.getData(1) & (1 << 0));
    ui->cmByte21->setChecked(beanFrame.getData(1) & (1 << 1));
    ui->cmByte22->setChecked(beanFrame.getData(1) & (1 << 2));
    ui->cmByte23->setChecked(beanFrame.getData(1) & (1 << 3));
    ui->cmByte24->setChecked(beanFrame.getData(1) & (1 << 4));
    ui->cmByte25->setChecked(beanFrame.getData(1) & (1 << 5));
    ui->cmByte26->setChecked(beanFrame.getData(1) & (1 << 6));
    ui->cbByte27->setChecked(beanFrame.getData(1) & (1 << 7));

    ui->cmByte30->setChecked(beanFrame.getData(2) & (1 << 0));
    ui->cmByte31->setChecked(beanFrame.getData(2) & (1 << 1));
    ui->cmByte32->setChecked(beanFrame.getData(2) & (1 << 2));
    ui->cmByte33->setChecked(beanFrame.getData(2) & (1 << 3));
    ui->cmByte34->setChecked(beanFrame.getData(2) & (1 << 4));
    ui->cmByte35->setChecked(beanFrame.getData(2) & (1 << 5));
    ui->cmByte36->setChecked(beanFrame.getData(2) & (1 << 6));
    ui->cbByte37->setChecked(beanFrame.getData(2) & (1 << 7));

    ui->cmByte40->setChecked(beanFrame.getData(3) & (1 << 0));
    ui->cmByte41->setChecked(beanFrame.getData(3) & (1 << 1));
    ui->cmByte42->setChecked(beanFrame.getData(3) & (1 << 2));
    ui->cmByte43->setChecked(beanFrame.getData(3) & (1 << 3));
    ui->cmByte44->setChecked(beanFrame.getData(3) & (1 << 4));
    ui->cmByte45->setChecked(beanFrame.getData(3) & (1 << 5));
    ui->cmByte46->setChecked(beanFrame.getData(3) & (1 << 6));
    ui->cbByte47->setChecked(beanFrame.getData(3) & (1 << 7));

    ui->cmByte50->setChecked(beanFrame.getData(4) & (1 << 0));
    ui->cmByte51->setChecked(beanFrame.getData(4) & (1 << 1));
    ui->cmByte52->setChecked(beanFrame.getData(4) & (1 << 2));
    ui->cmByte53->setChecked(beanFrame.getData(4) & (1 << 3));
    ui->cmByte54->setChecked(beanFrame.getData(4) & (1 << 4));
    ui->cmByte55->setChecked(beanFrame.getData(4) & (1 << 5));
    ui->cmByte56->setChecked(beanFrame.getData(4) & (1 << 6));
    ui->cbByte57->setChecked(beanFrame.getData(4) & (1 << 7));

    ui->cmByte60->setChecked(beanFrame.getData(5) & (1 << 0));
    ui->cmByte61->setChecked(beanFrame.getData(5) & (1 << 1));
    ui->cmByte62->setChecked(beanFrame.getData(5) & (1 << 2));
    ui->cmByte63->setChecked(beanFrame.getData(5) & (1 << 3));
    ui->cmByte64->setChecked(beanFrame.getData(5) & (1 << 4));
    ui->cmByte65->setChecked(beanFrame.getData(5) & (1 << 5));
    ui->cmByte66->setChecked(beanFrame.getData(5) & (1 << 6));
    ui->cbByte67->setChecked(beanFrame.getData(5) & (1 << 7));

    ui->cbByte70->setChecked(beanFrame.getData(6) & (1 << 0));
    ui->cbByte71->setChecked(beanFrame.getData(6) & (1 << 1));
    ui->cbByte72->setChecked(beanFrame.getData(6) & (1 << 2));
    ui->cbByte73->setChecked(beanFrame.getData(6) & (1 << 3));
    ui->cbByte74->setChecked(beanFrame.getData(6) & (1 << 4));
    ui->cbByte75->setChecked(beanFrame.getData(6) & (1 << 5));
    ui->cbByte76->setChecked(beanFrame.getData(6) & (1 << 6));
    ui->cbByte77->setChecked(beanFrame.getData(6) & (1 << 7));

    ui->cbByte80->setChecked(beanFrame.getData(7) & (1 << 0));
    ui->cbByte81->setChecked(beanFrame.getData(7) & (1 << 1));
    ui->cbByte82->setChecked(beanFrame.getData(7) & (1 << 2));
    ui->cbByte83->setChecked(beanFrame.getData(7) & (1 << 3));
    ui->cbByte84->setChecked(beanFrame.getData(7) & (1 << 4));
    ui->cbByte85->setChecked(beanFrame.getData(7) & (1 << 5));
    ui->cbByte86->setChecked(beanFrame.getData(7) & (1 << 6));
    ui->cbByte87->setChecked(beanFrame.getData(7) & (1 << 7));

    ui->cbByte90->setChecked(beanFrame.getData(8) & (1 << 0));
    ui->cbByte91->setChecked(beanFrame.getData(8) & (1 << 1));
    ui->cbByte92->setChecked(beanFrame.getData(8) & (1 << 2));
    ui->cbByte93->setChecked(beanFrame.getData(8) & (1 << 3));
    ui->cbByte94->setChecked(beanFrame.getData(8) & (1 << 4));
    ui->cbByte95->setChecked(beanFrame.getData(8) & (1 << 5));
    ui->cbByte96->setChecked(beanFrame.getData(8) & (1 << 6));
    ui->cbByte97->setChecked(beanFrame.getData(8) & (1 << 7));

    ui->cbByte100->setChecked(beanFrame.getData(9) & (1 << 0));
    ui->cbByte101->setChecked(beanFrame.getData(9) & (1 << 1));
    ui->cbByte102->setChecked(beanFrame.getData(9) & (1 << 2));
    ui->cbByte103->setChecked(beanFrame.getData(9) & (1 << 3));
    ui->cbByte104->setChecked(beanFrame.getData(9) & (1 << 4));
    ui->cbByte105->setChecked(beanFrame.getData(9) & (1 << 5));
    ui->cbByte106->setChecked(beanFrame.getData(9) & (1 << 6));
    ui->cbByte107->setChecked(beanFrame.getData(9) & (1 << 7));

    ui->cbByte110->setChecked(beanFrame.getData(10) & (1 << 0));
    ui->cbByte111->setChecked(beanFrame.getData(10) & (1 << 1));
    ui->cbByte112->setChecked(beanFrame.getData(10) & (1 << 2));
    ui->cbByte113->setChecked(beanFrame.getData(10) & (1 << 3));
    ui->cbByte114->setChecked(beanFrame.getData(10) & (1 << 4));
    ui->cbByte115->setChecked(beanFrame.getData(10) & (1 << 5));
    ui->cbByte116->setChecked(beanFrame.getData(10) & (1 << 6));
    ui->cbByte117->setChecked(beanFrame.getData(10) & (1 << 7));

    ui->cmByte10->blockSignals(false);
    ui->cmByte11->blockSignals(false);
    ui->cmByte12->blockSignals(false);
    ui->cmByte13->blockSignals(false);
    ui->cmByte14->blockSignals(false);
    ui->cmByte15->blockSignals(false);
    ui->cmByte16->blockSignals(false);
    ui->cbByte17->blockSignals(false);

    ui->cmByte20->blockSignals(false);
    ui->cmByte21->blockSignals(false);
    ui->cmByte22->blockSignals(false);
    ui->cmByte23->blockSignals(false);
    ui->cmByte24->blockSignals(false);
    ui->cmByte25->blockSignals(false);
    ui->cmByte26->blockSignals(false);
    ui->cbByte27->blockSignals(false);

    ui->cmByte30->blockSignals(false);
    ui->cmByte31->blockSignals(false);
    ui->cmByte32->blockSignals(false);
    ui->cmByte33->blockSignals(false);
    ui->cmByte34->blockSignals(false);
    ui->cmByte35->blockSignals(false);
    ui->cmByte36->blockSignals(false);
    ui->cbByte37->blockSignals(false);

    ui->cmByte40->blockSignals(false);
    ui->cmByte41->blockSignals(false);
    ui->cmByte42->blockSignals(false);
    ui->cmByte43->blockSignals(false);
    ui->cmByte44->blockSignals(false);
    ui->cmByte45->blockSignals(false);
    ui->cmByte46->blockSignals(false);
    ui->cbByte47->blockSignals(false);

    ui->cmByte50->blockSignals(false);
    ui->cmByte51->blockSignals(false);
    ui->cmByte52->blockSignals(false);
    ui->cmByte53->blockSignals(false);
    ui->cmByte54->blockSignals(false);
    ui->cmByte55->blockSignals(false);
    ui->cmByte56->blockSignals(false);
    ui->cbByte57->blockSignals(false);

    ui->cmByte60->blockSignals(false);
    ui->cmByte61->blockSignals(false);
    ui->cmByte62->blockSignals(false);
    ui->cmByte63->blockSignals(false);
    ui->cmByte64->blockSignals(false);
    ui->cmByte65->blockSignals(false);
    ui->cmByte66->blockSignals(false);
    ui->cbByte67->blockSignals(false);

    ui->cbByte70->blockSignals(false);
    ui->cbByte71->blockSignals(false);
    ui->cbByte72->blockSignals(false);
    ui->cbByte73->blockSignals(false);
    ui->cbByte74->blockSignals(false);
    ui->cbByte75->blockSignals(false);
    ui->cbByte76->blockSignals(false);
    ui->cbByte77->blockSignals(false);

    ui->cbByte80->blockSignals(false);
    ui->cbByte81->blockSignals(false);
    ui->cbByte82->blockSignals(false);
    ui->cbByte83->blockSignals(false);
    ui->cbByte84->blockSignals(false);
    ui->cbByte85->blockSignals(false);
    ui->cbByte86->blockSignals(false);
    ui->cbByte87->blockSignals(false);

    ui->cbByte90->blockSignals(false);
    ui->cbByte91->blockSignals(false);
    ui->cbByte92->blockSignals(false);
    ui->cbByte93->blockSignals(false);
    ui->cbByte94->blockSignals(false);
    ui->cbByte95->blockSignals(false);
    ui->cbByte96->blockSignals(false);
    ui->cbByte97->blockSignals(false);

    ui->cbByte100->blockSignals(false);
    ui->cbByte101->blockSignals(false);
    ui->cbByte102->blockSignals(false);
    ui->cbByte103->blockSignals(false);
    ui->cbByte104->blockSignals(false);
    ui->cbByte105->blockSignals(false);
    ui->cbByte106->blockSignals(false);
    ui->cbByte107->blockSignals(false);

    ui->cbByte110->blockSignals(false);
    ui->cbByte111->blockSignals(false);
    ui->cbByte112->blockSignals(false);
    ui->cbByte113->blockSignals(false);
    ui->cbByte114->blockSignals(false);
    ui->cbByte115->blockSignals(false);
    ui->cbByte116->blockSignals(false);
    ui->cbByte117->blockSignals(false);

    ui->cbPriority->setCurrentIndex(beanFrame.getPri());
    ui->cbDestId->setCurrentIndex(beanFrame.getDstId());
    ui->cbMessageId->setCurrentIndex(beanFrame.getMsgId());
    ui->sbBytesCount->setValue(beanFrame.getMl() - 2);

    updateBytes();
    updateFrameBytes();

    ui->cbMessageId->blockSignals(false);
    ui->cbDestId->blockSignals(false);
    ui->cbPriority->blockSignals(false);
    ui->sbBytesCount->blockSignals(false);
}

void MainWindow::on_tbToggleDbFrame_clicked(const QModelIndex &index)
{
    beanFrame = toggleDbFrameModel->getFrame(ui->tbToggleDbFrame->currentIndex().row());
    loadDbFrame();
}

void MainWindow::on_actionSaveDatabase_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save frame database"), "",
            tr("Frame database(*.framedb);;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }
           for(int i = 0; i < toggleDbFrameModel->rowCount(); i++) {
               BeanFrame f = toggleDbFrameModel->getFrame(i);
               file.write(reinterpret_cast<char*>(&f), sizeof(f));
           }
           file.flush();
           file.close();
    }
}

void MainWindow::on_actionOpenDatabase_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open frame database"), "",
            tr("Frame database(*.framedb);;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }
           toggleDbFrameModel->clear();
           BeanFrame f;
           while (file.read(reinterpret_cast<char*>(&f), sizeof(f)) != 0) {
                toggleDbFrameModel->addFrame(f);
           }
           file.flush();
           file.close();
           if (toggleDbFrameModel->rowCount() > 0) {
               ui->tbToggleDbFrame->selectRow(0);
               beanFrame = toggleDbFrameModel->getFrame(ui->tbToggleDbFrame->currentIndex().row());
               loadDbFrame();
               unlockToggleUI();
           }
    }
}

void MainWindow::on_btnSaveDump_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save dump"), "",
            tr("Dump(*.dat);;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }
           file.write(reinterpret_cast<char*>(&logStartMSec), sizeof(qint64));
           for(int i = 0; i < model->rowCount(); i++) {
               LoggerFrame* loggerFrame = model->getLoggerFrame(i);
               file.write(reinterpret_cast<char*>(loggerFrame), sizeof(LoggerFrame));
           }
           file.flush();
           file.close();
    }
}

void MainWindow::on_btnOpenDump_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open dump"), "",
            tr("Dump(*.dat);;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }
           model->clear();
           groupModel->clear();           
           BeanFrame f;
           LoggerFrame loggerFrame(f);
           file.read(reinterpret_cast<char*>(&logStartMSec), sizeof(qint64));
           while (file.read(reinterpret_cast<char*>(&loggerFrame), sizeof(LoggerFrame)) != 0) {
               model->putLoggerFrame(&loggerFrame);
               groupModel->putLoggerFrame(&loggerFrame);
           }
           file.flush();
           file.close();
           selectedRowNum = -1;
           ui->tblLogger->selectRow(0);
           ui->tblLoggerGroup->selectRow(0);
    }
}

void MainWindow::on_btnSendToPlayer_clicked()
{

    if (model->rowCount() == 0) {
        QMessageBox::warning(this, "Logger error", "Nothing to copy");
        return;
    }

    if (isPlaying) {
        QMessageBox::warning(this, "Logger error", "The player is active! Stop it and try again.");
        return;
    }

    playerModel->clear();
    for(int i = 0; i < model->rowCount(); i++)
    {
        playerModel->putFrame(BeanFrame(*model->getFrame(i)), model->getLoggerFrame(i)->getTimeMsec());
    }
    ui->tblPlayer->selectRow(0);

    QMessageBox::information(this, "Logger message", "Copy complete");
}

void MainWindow::onTimerShot(void)
{
    PlayerFrame* playerFrame = playerModel->getFrame(playerPosition);
    if (isSending) {
        timer->setInterval(playerFrame->getDelay());
        timer->setSingleShot(true);
        timer->start();
        return;
    }
    timeoutTimer->stop();
    if (isBeanFrameTransmitError) {
        playerFrame->setTxErr(1);
        playerFrame->setTxAck(0);
    } else {
        playerFrame->setTxErr(0);
        playerFrame->setTxAck(isBeanFrameTransmitAck ? 1 : 0);
    }
    ui->tblPlayer->selectRow(playerPosition);
    playerPosition++;
    if (playerPosition < playerModel->rowCount()) {
        playNext();
    } else {
        playerStop();
        QMessageBox::information(this, "Player message", "Play complete");
    }
}

void MainWindow::onTimerOneShot()
{
    PlayerFrame* playerFrame = playerModel->getFrame(playerPosition);
    if (isSending) {
        timer->setInterval(playerFrame->getDelay());
        timer->setSingleShot(true);
        timer->start();
        return;
    }
    timeoutTimer->stop();
    if (isBeanFrameTransmitError) {
        playerFrame->setTxErr(1);
        playerFrame->setTxAck(0);
    } else {
        playerFrame->setTxErr(0);
        playerFrame->setTxAck(isBeanFrameTransmitAck ? 1 : 0);
    }
    playerStop();
    playerPosition++;
    if (playerPosition < playerModel->rowCount()) {
       ui->tblPlayer->selectRow(playerPosition);
    }
}

void MainWindow::onTimeout()
{
    isBeanFrameTransmitError = true;
    isSending = false;
}

void MainWindow::playNext() {
    PlayerFrame* playerFrame = playerModel->getFrame(playerPosition);
    isBeanFrameTransmitAck = false;
    isBeanFrameTransmitError = false;
    deviceSend(playerFrame->getFrame());
    timeoutTimer->setInterval(playerFrame->getDelay() > 0 ? playerFrame->getDelay() + 100 : 1000);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start();
    timer->setInterval(playerFrame->getDelay());
    timer->setSingleShot(true);
    timer->start();
}

void MainWindow::playerStop()
{
    timer->stop();

    QObject::disconnect(timeoutTimer, &QTimer::timeout, this, &MainWindow::onTimeout);
    QObject::disconnect(timer, &QTimer::timeout, this, &MainWindow::onTimerShot);
    QObject::disconnect(timer, &QTimer::timeout, this, &MainWindow::onTimerOneShot);
    QObject::disconnect(device, &Device::beanFrameTransmitted, this, &MainWindow::beanFrameTransmitted);
    QObject::disconnect(device, &Device::beanFrameTransmitError, this, &MainWindow::beanFrameTransmitError);

    ui->btnPlay->setText("Start");
    ui->btnNext->setEnabled(true);
    isPlaying = false;
}

void MainWindow::beanFrameTransmitted(bool ack)
{
    isBeanFrameTransmitError = false;
    isBeanFrameTransmitAck = ack;
    isSending = false;
}

void MainWindow::beanFrameTransmitError()
{
    isBeanFrameTransmitError = true;
    isSending = false;
}

void MainWindow::on_btnPlay_clicked()
{
    if (!device->isConnected()) {
        QMessageBox::warning(this, "Player error", "Device is not connected");
        return;
    }

    if (playerModel->rowCount() == 0) {
        QMessageBox::warning(this, "Player error", "Nothing to play");
        return;
    }

    if (!isPlaying) {
        ui->btnPlay->setText("Stop");
        ui->btnNext->setEnabled(false);
        playerPosition = ui->tblPlayer->currentIndex().row();
        playerModel->resetFrames(playerPosition);
        QObject::connect(timer, &QTimer::timeout, this, &MainWindow::onTimerShot);
        QObject::connect(timeoutTimer, &QTimer::timeout, this, &MainWindow::onTimeout);
        QObject::connect(device, &Device::beanFrameTransmitted, this, &MainWindow::beanFrameTransmitted);
        QObject::connect(device, &Device::beanFrameTransmitError, this, &MainWindow::beanFrameTransmitError);
        isPlaying = true;
        playNext();
    } else {
        playerStop();
    }

}

void MainWindow::on_btnNext_clicked()
{

    if (!device->isConnected()) {
        QMessageBox::warning(this, "Player error", "Device is not connected");
        return;
    }

    if (playerModel->rowCount() == 0) {
        QMessageBox::warning(this, "Player error", "Nothing to play");
        return;
    }

    ui->btnPlay->setText("Stop");
    ui->btnNext->setEnabled(false);
    playerPosition = ui->tblPlayer->currentIndex().row();
    playerModel->resetFrame(playerPosition);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::onTimerOneShot);
    QObject::connect(timeoutTimer, &QTimer::timeout, this, &MainWindow::onTimeout);
    QObject::connect(device, &Device::beanFrameTransmitted, this, &MainWindow::beanFrameTransmitted);
    QObject::connect(device, &Device::beanFrameTransmitError, this, &MainWindow::beanFrameTransmitError);
    playNext();
}

void MainWindow::on_btnPrev_clicked()
{
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_cbByte17_stateChanged(int arg1)
{

}

void MainWindow::on_cbByte17_clicked()
{

}
