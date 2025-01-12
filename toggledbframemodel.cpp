#include "toggledbframemodel.h"

ToggleDbFrameModel::ToggleDbFrameModel(QObject* parent):
    QAbstractTableModel(parent)
{
}

int ToggleDbFrameModel::rowCount(const QModelIndex & /*parent*/) const
{
    return frames.length();
}

int ToggleDbFrameModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant ToggleDbFrameModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("DST");
        case 1:
            return QString("MSG");
        case 2:
            return QString("COMMENT");
        case 3:
            return QString("ACK");
        }
    }
    return QVariant();
}

QVariant ToggleDbFrameModel::data(const QModelIndex &index, int role) const
{

    const int row = index.row();

    if (role == Qt::DisplayRole) {

        BeanFrame* beanFrame = (BeanFrame*)&frames[row];

        switch (index.column()) {
        case 0:
            return QString("%1").arg(beanFrame->getDstId(), 2, 16, QChar('0')).toUpper();
        case 1:
            return QString("%1").arg(beanFrame->getMsgId(), 2, 16, QChar('0')).toUpper();
        case 2:
            return QString("Frame");
        }
    }

    return QVariant();
}

void ToggleDbFrameModel::addFrame(BeanFrame frame)
{
    frames.append(frame);
    emit layoutChanged();
}

void ToggleDbFrameModel::deleteFrame(QModelIndex index)
{
    frames.removeAt(index.row());
    emit layoutChanged();
}

BeanFrame ToggleDbFrameModel::getFrame(int rowNum)
{
    return frames.at(rowNum);
}

void ToggleDbFrameModel::updateFrame(QModelIndex index, BeanFrame frame)
{
    frames.replace(index.row(), frame);
    emit layoutChanged();
}

void ToggleDbFrameModel::clear()
{
    frames.clear();
    emit layoutChanged();
}
