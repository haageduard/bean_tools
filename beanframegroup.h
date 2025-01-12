#ifndef BEANFRAMEGROUP_H
#define BEANFRAMEGROUP_H

#include <stdint.h>

class BeanFrameGroup
{
public:
    BeanFrameGroup();

    uint8_t getDstId() const;
    void setDstId(uint8_t value);

    uint8_t getMsgId() const;
    void setMsgId(uint8_t value);

    int getCount() const;
    void setCount(int value);

    int getPeriod() const;
    void setPeriod(int value);

    long long getLastRxTime() const;
    void setLastRxTime(long long value);

private:
    uint8_t dstId;
    uint8_t msgId;
    int count;
    int period;
    long long lastRxTime;
};

#endif // BEANFRAMEGROUP_H
