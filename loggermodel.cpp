#include <QDebug>

#include "loggermodel.h"

LoggerModel::LoggerModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    isFiltered = false;
}

int LoggerModel::rowCount(const QModelIndex & /*parent*/) const
{
    return (isFiltered ? framesFiltered.length() : frames.length());
}

int LoggerModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 8;
}

QVariant LoggerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:            
            return QString("TIME");
        case 1:
            return QString("PRI");
        case 2:
            return QString("ML");
        case 3:
            return QString("DST");
        case 4:
            return QString("MSG");
        case 5:
            return QString("DATA");
        case 6:
            return QString("CRC");
        case 7:
            return QString("ACK");
        }
    }
    return QVariant();
}

QVariant LoggerModel::data(const QModelIndex &index, int role) const
{

    BeanFrame* frame;
    LoggerFrame* loggerFrame;
    const int row = index.row();

    if (role == Qt::DisplayRole) {


        if (!isFiltered) {
            loggerFrame = (LoggerFrame*)&frames[row];
        } else {
            loggerFrame = framesFiltered[row];
        }

        frame = loggerFrame->getFrame();

//        qDebug() << "update row at" << row << "column";


        switch (index.column()) {
        case 0:
            return loggerFrame->getTime().toString("dd.MM.yy hh:mm:ss.z");
        case 1:
            return QString("%1").arg(frame->getPri(), 2, 16, QChar('0')).toUpper();
        case 2:
            return QString("%1").arg(frame->getMl(), 2, 16, QChar('0')).toUpper();
        case 3:
            return QString("%1").arg(frame->getDstId(), 2, 16, QChar('0')).toUpper();
        case 4:
            return QString("%1").arg(frame->getMsgId(), 2, 16, QChar('0')).toUpper();
        case 5: {
            QString data;
            for(int i = 0; i < frame->getMl() - 2; i++) {
                data = data.append("%1 ").arg(frame->getData(i), 2, 16, QChar('0')).toUpper();
            }
            data = data.trimmed();
            return data;
            }
        case 6:
            return QString("%1").arg(frame->getCrc(), 2, 16, QChar('0')).toUpper();
        case 7:
            return QString("%1").arg(frame->getAck());
        }

    }

    return QVariant();
}

void LoggerModel::putFrame(BeanFrame* frame)
{
    BeanFrame f = *frame;    
    LoggerFrame loggerFrame(f);
    loggerFrame.setTime(QDateTime::currentDateTime());
    this->putLoggerFrame(&loggerFrame);
}

void LoggerModel::putLoggerFrame(LoggerFrame *loggerFrame)
{

    BeanFrame f = *loggerFrame->getFrame();

    int dst = f.getDstId();
    int msg = f.getMsgId();

    dstMsgFrames[dst][msg] = f;

    beginInsertRows(QModelIndex(), frames.size(), frames.size());
    frames.append(*loggerFrame);
    if (isFiltered) {
        filterFrame(&frames.last());
    }
    endInsertRows();
}

void LoggerModel::clear()
{
//    for(auto &dst: dstMsgFrames) {
//        dst.second.clear();
//        dstMsgFrames[dst.first].clear();
//    }

    frames.clear();
    framesFiltered.clear();
    emit layoutChanged();
}

void LoggerModel::setFilter(int msgId, int dstId)
{
    framesFiltered.clear();
    filterMsgId = msgId;
    filterDstId = dstId;
    if (msgId == 0xFF && dstId == 0xFF)  {
        isFiltered = false;
    } else {
        for(int i = 0; i < frames.length(); i++) {
            filterFrame((LoggerFrame*)&frames.at(i));
        }
        isFiltered = true;
    }

    emit layoutChanged();

}

BeanFrame* LoggerModel::getFrame(int rowNum)
{
    if (!isFiltered) {
        LoggerFrame loggerFrame = frames.at(rowNum);
        return loggerFrame.getFrame();
    } else {
        LoggerFrame* loggerFrame = framesFiltered.at(rowNum);
        return loggerFrame->getFrame();
    }
}

LoggerFrame *LoggerModel::getLoggerFrame(int rowNum)
{
    if (!isFiltered) {
        LoggerFrame* loggerFrame = &frames[rowNum];
        return loggerFrame;
    } else {
        LoggerFrame* loggerFrame = framesFiltered[rowNum];
        return loggerFrame;
    }
}

void LoggerModel::filterFrame(LoggerFrame *loggerFrame)
{

    BeanFrame* frame = loggerFrame->getFrame();

    if ((filterMsgId != -1 && frame->getMsgId() == filterMsgId) && ((filterDstId == -1))) {
        framesFiltered.append(loggerFrame);
    } else if ((filterDstId != -1 && frame->getDstId() == filterDstId) && (filterMsgId == -1)) {
        framesFiltered.append(loggerFrame);
    } else if (((frame->getMsgId() == filterMsgId) &&
                       (frame->getDstId() == filterDstId)) || (filterMsgId == 0xFF || filterDstId == 0xFF))  {
        framesFiltered.append(loggerFrame);
    }
}

BeanFrame* LoggerModel::getFrameByDstMsg(int dst, int msg) {
    BeanFrame *f = &dstMsgFrames[dst][msg];
    return f;
}
