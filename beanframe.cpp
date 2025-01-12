#include "beanframe.h"

BeanFrame::BeanFrame()
{

}

uint8_t BeanFrame::getPri() const
{
    return pri;
}

void BeanFrame::setPri(uint8_t value)
{
    pri = value;
}

uint8_t BeanFrame::getMl() const
{
    return ml;
}

void BeanFrame::setMl(uint8_t value)
{
    ml = value;
}

uint8_t BeanFrame::getDstId() const
{
    return dstId;
}

void BeanFrame::setDstId(uint8_t value)
{
    dstId = value;
}

uint8_t BeanFrame::getMsgId() const
{
    return msgId;
}

void BeanFrame::setMsgId(uint8_t value)
{
    msgId = value;
}

uint8_t BeanFrame::getData(uint8_t i) const {
    return data[i];
}
void BeanFrame::setData(uint8_t index, uint8_t value) {
    data[index] = value;
}

uint8_t BeanFrame::getCrc() const
{
    return crc;
}

void BeanFrame::setCrc(uint8_t value)
{
    crc = value;
}

uint8_t BeanFrame::getAck() const
{
    return ack;
}

void BeanFrame::setAck(uint8_t value)
{
    ack = value;
}
