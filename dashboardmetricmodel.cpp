#include <QDebug>

#include "dashboardmetricmodel.h"

DashboardMetricModel::DashboardMetricModel(QObject* parent):
      QAbstractTableModel(parent)
{

}

int DashboardMetricModel::rowCount(const QModelIndex & /*parent*/) const
{
    return items.length();
}

int DashboardMetricModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant DashboardMetricModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("METRIC");
        case 1:
            return QString("VALUE");
        }
    }
    return QVariant();
}

QVariant DashboardMetricModel::data(const QModelIndex &index, int role) const
{

    const int row = index.row();

    if (role == Qt::DisplayRole) {

        DashboardMetric* metric = items.at(row);

//        qDebug() << "update metric" << metric->name << "at" << row << "column" << index.column() << "value" << metric->value;

        switch (index.column()) {
        case 0:
            return metric->name;
        case 1:
            return metric->value;
        }

    }

    return QVariant();
}


void DashboardMetricModel::putItem(DashboardMetric *m)
{
    items.append(m);
    QModelIndex startOfRow = this->index(items.length()-1, 0);
    QModelIndex endOfRow   = this->index(items.length()-1, columnCount());
    emit QAbstractItemModel::dataChanged(startOfRow, endOfRow);
    emit QAbstractItemModel::layoutChanged();
}

void DashboardMetricModel::clear() {
    items.clear();
    emit  QAbstractItemModel::layoutChanged();
}

void DashboardMetricModel::updateItem(DashboardMetric *m)
{
    int index = items.indexOf(m);
    if (index == -1) {
        return;
    }
    QModelIndex startOfRow = this->index(index, 0);
    QModelIndex endOfRow   = this->index(index, columnCount());
//    qDebug() << "updateItem";
    emit QAbstractItemModel::dataChanged(startOfRow, endOfRow);
    emit QAbstractItemModel::layoutChanged();
}
