#ifndef BEANFRAME_H
#define BEANFRAME_H

#define BEAN_MAX_DATA_LEN 11

#include <stdint.h>

class BeanFrame
{
public:
    BeanFrame();
    uint8_t getPri() const;
    void setPri(uint8_t value);

    uint8_t getMl() const;
    void setMl(uint8_t value);

    uint8_t getDstId() const;
    void setDstId(uint8_t value);

    uint8_t getMsgId() const;
    void setMsgId(uint8_t value);

    uint8_t getCrc() const;
    void setCrc(uint8_t value);

    uint8_t getData(uint8_t i) const;
    void setData(uint8_t index, uint8_t value);

    uint8_t getAck() const;
    void setAck(uint8_t value);

private:
    uint8_t pri;
    uint8_t ml;
    uint8_t dstId;
    uint8_t msgId;
    uint8_t data[BEAN_MAX_DATA_LEN];
    uint8_t crc;
    uint8_t ack;
};

#endif // BEANFRAME_H
