#ifndef DASHBOARDMETRICMODEL_H
#define DASHBOARDMETRICMODEL_H

#include <QAbstractTableModel>

#include "dashboard.h"

class DashboardMetricModel : public QAbstractTableModel
{
     Q_OBJECT
public:
    DashboardMetricModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void putItem(DashboardMetric *m);
    void updateItem(DashboardMetric *m);
    void clear(void);
private:
     QList<DashboardMetric*> items;
};

#endif // DASHBOARDMETRICMODEL_H
