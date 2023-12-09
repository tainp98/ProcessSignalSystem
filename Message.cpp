#include "Message.h"

Message::Message(SensorType sensorType)
    : mSensorType(sensorType)
{

}

uint32_t Message::seqNumber() const
{
    return mSeqNumber;
}

void Message::setSeqNumber(uint32_t seq)
{
    mSeqNumber = seq;
}

uint16_t Message::ackNumber() const
{
    return mAckNumber;
}

void Message::setAckNumber(uint16_t ack)
{
    mAckNumber = ack;
}

std::vector<char> &Message::payload()
{
    return mPayload;
}

void Message::setPayload(const std::vector<char> &payload)
{
    mPayload = payload;
}

SensorType Message::sensorType() const
{
    return mSensorType;
}
