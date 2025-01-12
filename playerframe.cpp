#include "playerframe.h"

PlayerFrame::PlayerFrame()
{

}

BeanFrame* PlayerFrame::getFrame()
{
    return &frame;
}

void PlayerFrame::setFrame(const BeanFrame &value)
{
    frame = value;
}

uint8_t PlayerFrame::getTxAck() const
{
    return txAck;
}

void PlayerFrame::setTxAck(const uint8_t &value)
{
    txAck = value;
}

long long PlayerFrame::getTimeMsec() const
{
    return timeMsec;
}

void PlayerFrame::setTimeMsec(long long value)
{
    timeMsec = value;
}

uint8_t PlayerFrame::getTxErr() const
{
    return txErr;
}

void PlayerFrame::setTxErr(const uint8_t &value)
{
    txErr = value;
}

int PlayerFrame::getDelay() const
{
    return delay;
}

void PlayerFrame::setDelay(int value)
{
    delay = value;
}
