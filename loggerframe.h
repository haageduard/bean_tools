#ifndef LOGGERFRAME_H
#define LOGGERFRAME_H

#include <QDateTime>

#include "beanframe.h"

class LoggerFrame
{
public:
    LoggerFrame(BeanFrame f);
    QDateTime getTime() const;
    void setTime(const QDateTime &value);

    BeanFrame* getFrame();
    void setFrame(BeanFrame value);

    qint64 getTimeMsec() const;
    void setTimeMsec(const qint64 &value);

private:
    QDateTime time;
    BeanFrame frame;
    long long timeMsec;
};

#endif // LOGGERFRAME_H
