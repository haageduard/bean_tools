#ifndef LOGGERMODEL_H
#define LOGGERMODEL_H

#include <QAbstractTableModel>

#include "loggerframe.h"
#include "beanframe.h"

class LoggerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LoggerModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void putFrame(BeanFrame* frame);
    void putLoggerFrame(LoggerFrame* loggerFrame);
    void clear(void);
    void setFilter(int msgId, int dstId);
    BeanFrame* getFrame(int rowNum);
    LoggerFrame* getLoggerFrame(int rowNum);
private:
    bool isFiltered;
    int filterMsgId;
    int filterDstId;
    QList<LoggerFrame> frames;
    QList<LoggerFrame*> framesFiltered;
    void filterFrame(LoggerFrame* frame);
};

#endif // LOGGERMODEL_H
