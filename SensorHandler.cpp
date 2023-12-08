#include "SensorHandler.h"

SensorHandler::SensorHandler()
{
    mPackageEncap = std::make_shared<PackageEncapsulation>();
    mSenderSocket = std::make_shared<BluezSocket>();
    tGenerateSignal1 = std::thread(&SensorHandler::generateSignal1, this);
    tGenerateSignal2 = std::thread(&SensorHandler::generateSignal2, this);
}

SensorHandler::~SensorHandler()
{
    if(tGenerateSignal1.joinable()) tGenerateSignal1.join();
    if(tGenerateSignal2.joinable()) tGenerateSignal2.join();
}

void SensorHandler::generateSignal1()
{
    std::vector<char> vec(3);
    while (true) {
        vec[0] = 0x01;
        vec[1] = 0x02;
        vec[2] = 0x03;
        std::unique_ptr<Message> msg = std::make_unique<Message>(SensorType::SENSOR1);
        msg->setSeqNumber(mSeqNumber1++);
        msg->setAckNumber(mAckNumber1++);
        msg->setPayload(vec);
        // encap package and send to phone device
        mSenderSocket->write(mPackageEncap->toBytes(std::move(msg)));
        std::this_thread::sleep_for(std::chrono::microseconds(1000000/SAMPLERATE1));
    }
}

void SensorHandler::generateSignal2()
{
    std::vector<char> vec(3);
    while (true) {
        vec[0] = 0x04;
        vec[1] = 0x05;
        vec[2] = 0x06;
        std::unique_ptr<Message> msg = std::make_unique<Message>(SensorType::SENSOR2);
        msg->setSeqNumber(mSeqNumber2++);
        msg->setAckNumber(mAckNumber2++);
        msg->setPayload(vec);
        // encap package and send to phone device
        mSenderSocket->write(mPackageEncap->toBytes(std::move(msg)));
        std::this_thread::sleep_for(std::chrono::microseconds(1000000/SAMPLERATE2));
    }
}
