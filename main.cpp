#include <iostream>
#include "PhoneReceiver.h"
#include "SensorHandler.h"

using namespace std;
struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}

void printBytes(const std::vector<char>& data){
    for(int i = 0; i < data.size(); i++){
        std::cout<<hex(data[i]);
        if(i < data.size()-1) std::cout<<" - ";
    }
    std::cout<<"\n";
}

int main()
{
    // test encapsulate package
    PackageEncapsulation packageEncap;
    std::vector<char> vec(3);
    vec[0] = 0x01;
    vec[1] = 0x02;
    vec[2] = 0x03;
    uint16_t seqNum = 1, ackNum = 1;
    std::unique_ptr<Message> msg1 = std::make_unique<Message>(SensorType::SENSOR1);
    std::unique_ptr<Message> msg2 = std::make_unique<Message>(SensorType::SENSOR2);
    msg1->setSeqNumber(seqNum);
    msg1->setAckNumber(ackNum);
    msg1->setPayload(vec);
    msg2->setSeqNumber(seqNum);
    msg2->setAckNumber(ackNum);
    msg2->setPayload(vec);
    std::cout<<"------- encap Message to byte array ------\n";
    auto msg1Byte = packageEncap.toBytes(std::move(msg1));
    printBytes(msg1Byte);
    auto msg2Byte = packageEncap.toBytes(std::move(msg2));
    printBytes(msg2Byte);

    // re-convert to Message
    std::cout<<"\n------- revert byte array to Message ------\n";
    auto reMsg1 = packageEncap.toMessage(msg1Byte);
    auto reMsg2 = packageEncap.toMessage(msg2Byte);
    std::cout<<static_cast<int>(reMsg1->sensorType())<<" - "<<reMsg1->seqNumber()
            <<" - "<<reMsg1->ackNumber()<<"\n";
    printBytes(reMsg1->payload());
    std::cout<<static_cast<int>(reMsg2->sensorType())<<" - "<<reMsg2->seqNumber()
            <<" - "<<reMsg2->ackNumber()<<"\n";
    printBytes(reMsg2->payload());
    return 0;
}
