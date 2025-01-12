#include "loggergroupmodel.h"

LoggerGroupModel::LoggerGroupModel(QObject* parent):
    QAbstractTableModel(parent)
{
    addAllGroups();
}

int LoggerGroupModel::rowCount(const QModelIndex & /*parent*/) const
{
    return frameGroup.length();
}

int LoggerGroupModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant LoggerGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("DST");
        case 1:
            return QString("MSG");
        case 2:
            return QString("COUNT");
        case 3:
            return QString("PERIOD");
        }
    }
    return QVariant();
}

QVariant LoggerGroupModel::data(const QModelIndex &index, int role) const
{

    const int row = index.row();

    if (role == Qt::DisplayRole) {

        BeanFrameGroup fg = frameGroup.at(row);

        switch (index.column()) {
        case 0:
            return (index.row() == 0 ? QString("*") : QString("%1").arg(fg.getDstId(), 2, 16, QChar('0')).toUpper());
        case 1:
            return (index.row() == 0 ? QString("*") : QString("%1").arg(fg.getMsgId(), 2, 16, QChar('0')).toUpper());
        case 2:
            return QString("%1").arg(index.row() == 0 ? allGroups.getCount() : fg.getCount());
        case 3:
            return (index.row() == 0 ? QVariant() : QString("%1").arg(fg.getPeriod()));
        }

    }

    return QVariant();
}

bool LoggerGroupModel::lessDst(const BeanFrameGroup &f1, const BeanFrameGroup &f2)
{
    return (f1.getDstId() < f2.getDstId());
}

bool LoggerGroupModel::lessMsgByDst(const BeanFrameGroup &f1, const BeanFrameGroup &f2)
{
    return ((f1.getDstId() == f2.getDstId()) && (f1.getMsgId() < f2.getMsgId()));
}

BeanFrameGroup *LoggerGroupModel::getFrameGroup(const int row)
{
    return &frameGroup[row];
}

void LoggerGroupModel::putLoggerFrame(LoggerFrame* loggerFrame)
{
    BeanFrame* frame = loggerFrame->getFrame();
    bool found = false;
    for(int i = 0; i < frameGroup.length(); i++) {
        if (frameGroup.at(i).getDstId() == frame->getDstId() &&
            frameGroup.at(i).getMsgId() == frame->getMsgId()) {
            frameGroup[i].setCount(frameGroup.at(i).getCount() + 1);
            frameGroup[i].setPeriod((frameGroup[i].getPeriod() + loggerFrame->getTimeMsec()) / frameGroup[i].getCount() - 1);
            found = true;
            break;
        }
    }
    if (!found) {
        BeanFrameGroup fg;
        fg.setDstId(frame->getDstId());
        fg.setMsgId(frame->getMsgId());
        fg.setCount(1);
        fg.setPeriod(0);
        frameGroup.append(fg);
    }

    allGroups.setCount(allGroups.getCount() + 1);

//    std::sort(frameGroup.begin(), frameGroup.end(), lessDst);
//    std::sort(frameGroup.begin(), frameGroup.end(), lessMsgByDst);

//    emit layoutChanged();

    QModelIndex startOfRow = this->index(frameGroup.length()-1, 0);
    QModelIndex endOfRow   = this->index(frameGroup.length()-1, columnCount());
    emit QAbstractItemModel::dataChanged(startOfRow, endOfRow);
    emit QAbstractItemModel::layoutChanged();
}

void LoggerGroupModel::clear()
{
    frameGroup.clear();
    addAllGroups();
    emit layoutChanged();
}

void LoggerGroupModel::addAllGroups()
{
    allGroups.setDstId(-1);
    allGroups.setMsgId(-1);
    allGroups.setCount(0);
    allGroups.setPeriod(0);
    frameGroup.append(allGroups);
}
