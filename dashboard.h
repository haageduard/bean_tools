#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QString>
#include <QJSEngine>

#include "beanframe.h"

struct ActionMessage {
    int priority;
    int dstId;
    int msgId;
    QVector<QString> data;
} ;

struct DashboardAction {
    QString name;
    QString icon;
    bool enabled;
    QVector<ActionMessage> message;
};

struct DashboardMetric {

    QString name;
    bool enabled;
    int dstId;
    int msgId;
    QString expr;
    std::map<QString, QString> matchValue;
    QString output;

    QString value;
};

class Dashboard
{

private:
    QJSEngine js;
public:
    QString name;
    int version;
    QList<DashboardMetric> metric;
    QList<DashboardAction> action;
    Dashboard();
    bool readFromFile(const QString &filePath);
    QList<DashboardMetric*> putFrame(BeanFrame* f);
};

#endif // DASHBOARD_H
