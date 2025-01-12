
#include "loggerframe.h"

LoggerFrame::LoggerFrame(BeanFrame f)
{
    this->frame = f;
}

QDateTime LoggerFrame::getTime() const
{
    return time;
}

void LoggerFrame::setTime(const QDateTime &value)
{
    time = value;
}

BeanFrame* LoggerFrame::getFrame()
{
    return &frame;
}

void LoggerFrame::setFrame(BeanFrame value)
{
    frame = value;
}

qint64 LoggerFrame::getTimeMsec() const
{
    return timeMsec;
}

void LoggerFrame::setTimeMsec(const qint64 &value)
{
    timeMsec = value;
}
