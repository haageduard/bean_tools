#ifndef TOGGLEFRAMEMODEL_H
#define TOGGLEFRAMEMODEL_H

#include <QAbstractTableModel>

#include "beanframe.h"
#include "beanframegroup.h"

class ToggleDbFrameModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    ToggleDbFrameModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addFrame(BeanFrame frame);
    void deleteFrame(QModelIndex index);
    BeanFrame getFrame(int rowNum);
    void updateFrame(QModelIndex index, BeanFrame frame);
    void clear();
private:
    QList<BeanFrame> frames;
};

#endif // TOGGLEFRAMEMODEL_H
