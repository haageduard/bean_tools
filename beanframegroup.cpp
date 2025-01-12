#include "beanframegroup.h"

BeanFrameGroup::BeanFrameGroup()
{

}

uint8_t BeanFrameGroup::getDstId() const
{
    return dstId;
}

void BeanFrameGroup::setDstId(uint8_t value)
{
    dstId = value;
}

uint8_t BeanFrameGroup::getMsgId() const
{
    return msgId;
}

void BeanFrameGroup::setMsgId(uint8_t value)
{
    msgId = value;
}

int BeanFrameGroup::getCount() const
{
    return count;
}

void BeanFrameGroup::setCount(int value)
{
    count = value;
}

int BeanFrameGroup::getPeriod() const
{
    return period;
}

void BeanFrameGroup::setPeriod(int value)
{
    period = value;
}

long long BeanFrameGroup::getLastRxTime() const
{
    return lastRxTime;
}

void BeanFrameGroup::setLastRxTime(long long value)
{
    lastRxTime = value;
}
