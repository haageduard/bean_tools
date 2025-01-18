#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "dashboard.h"

Dashboard::Dashboard()
{        

}


bool Dashboard::readFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open((QIODevice::ReadOnly))) {
        qDebug() << "Couldn't open the file" << filePath;
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (!jsonDoc.isObject()) {
        return false;
    }

    metric.clear();
    action.clear();

    bool ok = true;

    QJsonObject obj = jsonDoc.object();

    version = obj["version"].toInt();
    name = obj["name"].toString();

    QJsonArray actions = obj["action"].toArray();
    for (const QJsonValue &value: actions) {
        QJsonObject actionObj = value.toObject();
        DashboardAction a;
        a.name = actionObj["name"].toString();
        a.icon = actionObj["icon"].toString();
        a.enabled = actionObj["enabled"].toBool();
        QJsonArray message =  actionObj["message"].toArray();
        for (auto msg: message) {
            QJsonObject messageObj =  msg.toObject();
            ActionMessage am;
            am.dstId = messageObj["dst_id"].toString().toInt(&ok, 16);
            if (!ok) {
                return false;
            }
            am.msgId = messageObj["msg_id"].toString().toInt(&ok, 16);
            if (!ok) {
                return false;
            }
            am.priority = messageObj["prio"].toInt();
            QJsonArray data = messageObj["data"].toArray();
            for (const auto &dataValue: data) {
                QString tmp;
                if (dataValue.isString()) {
                    tmp = dataValue.toString();
                } else {
                    tmp = QString("%1").arg(dataValue.toInt());
                }
                am.data.append(tmp);
            }
            a.message.append(am);
        }
        action.append(a);
    }


    QJsonArray metrics = obj["metric"].toArray();
    for (const QJsonValue &value: metrics) {
        QJsonObject metricObj = value.toObject();
        DashboardMetric m;
        m.name = metricObj["name"].toString();
        m.enabled = metricObj["enabled"].toBool();
        m.dstId = metricObj["dst_id"].toString().toInt(&ok, 16);
        if (!ok) {
            return false;
        }
        m.msgId = metricObj["msg_id"].toString().toInt(&ok, 16);
        if (!ok) {
            return false;
        }
        m.expr = metricObj["expr"].toString();
        m.output = metricObj["output"].toString();
        QJsonValue matchValueValue =  metricObj["match_value"];
        if (matchValueValue.isObject()) {
            QJsonObject matchValueObj =  matchValueValue.toObject();
            for (const auto &key: matchValueObj.keys()) {
                QString k = key;
                if (k.startsWith("0x") ) {
                    k = QString("%1").arg(key.toInt(&ok, 16));
                }
                m.matchValue[k] =  matchValueObj.value(key).toString();
            }
        }
        m.value = "?";
        metric.append(m);
    }

    return true;
}


QList<DashboardMetric*> Dashboard::putFrame(BeanFrame* f) {

    QList<DashboardMetric*> resultMetric;

    for(DashboardMetric &m: metric) {
        if (!m.enabled) {
            continue;
        }
        if (m.dstId != f->getDstId()) {
            continue;
        }
        if (m.msgId != f->getMsgId()) {
            continue;
        }
        QString tmp = m.expr;
        for(int i = 0; i < f->getMl() - 2; i++) {
            tmp = tmp.replace(QString("$%1").arg(i), QString("%1").arg(f->getData(i)), Qt::CaseInsensitive);
        }
        qDebug() << "metric" << m.name << "expr=" << tmp;
        QJSValue jsValue =  js.evaluate(tmp);
        QString value = jsValue.toString();
        qDebug() << "metric" << m.name << "js_expr=" << value;
        QString result;
        if (m.matchValue.size()) {
            if (m.matchValue[value] != NULL) {
                qDebug() << "metric" << m.name << "value" << value << "matched for" << m.matchValue[value];
                value = m.matchValue[value];
            }
        }
        if (m.output.length()) {
            tmp = m.output;
            value = tmp.replace("$value", value, Qt::CaseInsensitive);
        }
        m.value = value;
        qDebug() << "metric" << m.name << "value" << m.value;
        resultMetric.append(&m);
    }

    return resultMetric;
}
