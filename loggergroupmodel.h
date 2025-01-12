#ifndef LOGGERGROUPMODEL_H
#define LOGGERGROUPMODEL_H

#include <QAbstractTableModel>

#include "beanframe.h"
#include "beanframegroup.h"
#include "loggerframe.h"

class LoggerGroupModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    LoggerGroupModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void putLoggerFrame(LoggerFrame* loggerFrame);
    void clear(void);
    BeanFrameGroup* getFrameGroup(const int row);
private:
    QList<BeanFrameGroup> frameGroup;
    BeanFrameGroup allGroups;
    void addAllGroups();
    static bool lessDst(const BeanFrameGroup &f1, const BeanFrameGroup &f2);
    static bool lessMsgByDst(const BeanFrameGroup &f1, const BeanFrameGroup &f2);

};

#endif // LOGGERGROUPMODEL_H
