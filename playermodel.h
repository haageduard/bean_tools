#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QAbstractTableModel>

#include "beanframe.h"
#include "playerframe.h"

class PlayerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PlayerModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void putFrame(BeanFrame frame, long long timeMsec);
    void clear(void);
    PlayerFrame* getFrame(int rowNum);
    void resetFrames(int startPosition);
    void resetFrame(int position);
private:
    QList<PlayerFrame> frames;
};

#endif // PLAYERMODEL_H
