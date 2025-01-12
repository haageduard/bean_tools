#include "playermodel.h"

PlayerModel::PlayerModel(QObject* parent)
: QAbstractTableModel(parent)
{

}

int PlayerModel::rowCount(const QModelIndex & /*parent*/) const
{
    return frames.length();
}

int PlayerModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 10;
}

QVariant PlayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("PRI");
        case 1:
            return QString("ML");
        case 2:
            return QString("DST");
        case 3:
            return QString("MSG");
        case 4:
            return QString("DATA");
        case 5:
            return QString("CRC");
        case 6:
            return QString("ACK");
        case 7:
            return QString("TX ACK");
        case 8:
            return QString("TX ERR");
        case 9:
            return QString("DELAY");
        }
    }
    return QVariant();
}

QVariant PlayerModel::data(const QModelIndex &index, int role) const
{

    BeanFrame* frame;
    PlayerFrame playerFrame;
    const int row = index.row();

    if (role == Qt::DisplayRole) {

        playerFrame = frames.at(row);
        frame = playerFrame.getFrame();

        switch (index.column()) {
        case 0:
            return QString("%1").arg(frame->getPri(), 2, 16, QChar('0')).toUpper();
        case 1:
            return QString("%1").arg(frame->getMl(), 2, 16, QChar('0')).toUpper();
        case 2:
            return QString("%1").arg(frame->getDstId(), 2, 16, QChar('0')).toUpper();
        case 3:
            return QString("%1").arg(frame->getMsgId(), 2, 16, QChar('0')).toUpper();
        case 4: {
            QString data;
            for(int i = 0; i < frame->getMl() - 2; i++) {
                data = data.append("%1 ").arg(frame->getData(i), 2, 16, QChar('0')).toUpper();
            }
            data = data.trimmed();
            return data;
        }
        case 5:
            return QString("%1").arg(frame->getCrc(), 2, 16, QChar('0')).toUpper();
        case 6:
            return QString("%1").arg(frame->getAck());
        case 7:
            return QString("%1").arg(playerFrame.getTxAck());
        case 8:
            return QString("%1").arg(playerFrame.getTxErr());
        case 9:
            return QString("%1").arg(playerFrame.getDelay());
        }


    }

    return QVariant();
}

void PlayerModel::putFrame(BeanFrame frame, long long timeMsec)
{
    PlayerFrame playerFrame;
    playerFrame.setFrame(frame);
    playerFrame.setTxAck(0);
    playerFrame.setTxErr(0);
    playerFrame.setTimeMsec(timeMsec);
    int delay = 0;
    if (frames.length() > 0) {
        delay = playerFrame.getTimeMsec() - frames.last().getTimeMsec();
    }
    playerFrame.setDelay(delay);
    frames.append(playerFrame);
    emit layoutChanged();
}

void PlayerModel::clear(void)
{
    frames.clear();
    emit layoutChanged();
}

PlayerFrame *PlayerModel::getFrame(int rowNum)
{
    return &frames[rowNum];
}

void PlayerModel::resetFrames(int startPosition)
{
    for(int i = startPosition; i < frames.length(); i++) {
        frames[i].setTxAck(0);
        frames[i].setTxErr(0);
    }
    emit layoutChanged();
}

void PlayerModel::resetFrame(int position)
{
    frames[position].setTxAck(0);
    frames[position].setTxErr(0);
    emit layoutChanged();
}
