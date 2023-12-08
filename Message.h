#ifndef MESSAGE_H
#define MESSAGE_H

#include <define.h>
#include <vector>
class Message
{
public:
    Message(SensorType sensorType);
    uint16_t seqNumber() const;
    void setSeqNumber(uint16_t seq);
    uint16_t ackNumber() const;
    void setAckNumber(uint16_t ack);
    std::vector<char>& payload();
    void setPayload(const std::vector<char>& payload);
    SensorType sensorType() const;
protected:
    uint16_t mSeqNumber{0};
    uint16_t mAckNumber{0};
    std::vector<char> mPayload;
    SensorType mSensorType;
};

#endif // MESSAGE_H