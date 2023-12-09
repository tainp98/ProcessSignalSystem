#include "PackageEncapsulation.h"

PackageEncapsulation::PackageEncapsulation()
{

}

std::vector<char> PackageEncapsulation::toBytes(std::unique_ptr<Message> msg)
{
    std::vector<char> res;
    res.reserve(TOTAL_SIZE_NO_PAYLOAD + msg->payload().size());
    res.push_back(HEADER1);
    res.push_back(HEADER2);
    std::vector<char> byteVec;
    byteVec = toBytes<uint8_t>(static_cast<uint8_t>(msg->payload().size()));
    res.insert(res.end(), byteVec.begin(), byteVec.end());
    byteVec = toBytes<uint32_t>(msg->seqNumber());
    res.insert(res.end(), byteVec.begin(), byteVec.end());
    byteVec = toBytes<uint16_t>(msg->ackNumber());
    res.insert(res.end(), byteVec.begin(), byteVec.end());
    byteVec = toBytes<SensorType>(msg->sensorType());
    res.insert(res.end(), byteVec.begin(), byteVec.end());
    res.insert(res.end(), msg->payload().begin(), msg->payload().end());
    // calculate checksum number
    char checksum = gencrc(res.data(), res.size());
    res.push_back(checksum);
    return res;
}

std::unique_ptr<Message> PackageEncapsulation::toMessage(const std::vector<char>& data)
{
    mAccumulatingVector.insert(mAccumulatingVector.end(), data.begin(), data.end());
    if(mAccumulatingVector.size() < TOTAL_SIZE_NO_PAYLOAD) return nullptr;
    while (mAccumulatingVector.size() >= TOTAL_SIZE_NO_PAYLOAD) {
        if(mAccumulatingVector[0] == HEADER1 && mAccumulatingVector[1] == HEADER2) break;
        else mAccumulatingVector.erase(mAccumulatingVector.begin());
    }
    if(mAccumulatingVector.size() < TOTAL_SIZE_NO_PAYLOAD) return nullptr;
    // convert to message
    uint8_t payloadLen = toValue<uint8_t>(
                std::vector<char>(mAccumulatingVector.begin()+HEADER_LEN, mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN)
                );
    if(mAccumulatingVector.size() < TOTAL_SIZE_NO_PAYLOAD+payloadLen) return nullptr;
    // calculate checksum
    char checksum = gencrc(mAccumulatingVector.data(), TOTAL_SIZE_NO_PAYLOAD+payloadLen-1);
    if(checksum != mAccumulatingVector[TOTAL_SIZE_NO_PAYLOAD+payloadLen-1]){
        std::cout<<"checksum doesn't match\n";
        mAccumulatingVector.erase(mAccumulatingVector.begin(), mAccumulatingVector.begin()+TOTAL_SIZE_NO_PAYLOAD+payloadLen);
        return nullptr;
    }
    SensorType sensorType = toValue<SensorType>(
                std::vector<char>(mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN+ACK_NUM_LEN,
                                  mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN+ACK_NUM_LEN+SENSORTYPE_LEN)
                );
    uint16_t seqNumber = toValue<uint32_t>(
                std::vector<char>(mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN,
                                  mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN)
                );
    uint16_t ackNumber = toValue<uint16_t>(
                std::vector<char>(mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN,
                                  mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN+ACK_NUM_LEN)
                );
    std::vector<char> payload = std::vector<char>(mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN+ACK_NUM_LEN+SENSORTYPE_LEN,
                                                  mAccumulatingVector.begin()+HEADER_LEN+LENGTH_LEN+SEQ_NUM_LEN+ACK_NUM_LEN+SENSORTYPE_LEN+payloadLen);
    std::unique_ptr<Message> msg = std::make_unique<Message>(sensorType);
    msg->setSeqNumber(seqNumber);
    msg->setAckNumber(ackNumber);
    msg->setPayload(std::move(payload));
    mAccumulatingVector.erase(mAccumulatingVector.begin(), mAccumulatingVector.begin()+TOTAL_SIZE_NO_PAYLOAD+payloadLen);
    return msg;
}

char PackageEncapsulation::gencrc(char *data, size_t len)
{
    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else
                crc <<= 1;
        }
    }
    return static_cast<char>(crc);
}
