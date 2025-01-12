#ifndef PLAYERFRAME_H
#define PLAYERFRAME_H

#include "beanframe.h"

class PlayerFrame
{
public:
    PlayerFrame();
    BeanFrame* getFrame();
    void setFrame(const BeanFrame &value);

    uint8_t getTxAck() const;
    void setTxAck(const uint8_t &value);

    int getDelay() const;
    void setDelay(int value);

    long long getTimeMsec() const;
    void setTimeMsec(long long value);

    uint8_t getTxErr() const;
    void setTxErr(const uint8_t &value);

private:
    BeanFrame frame;
    uint8_t txAck;
    uint8_t txErr;
    long long timeMsec;
    int delay;
};

#endif // PLAYERFRAME_H
