#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H
#include "BluezSocket.h"
#include "PackageEncapsulation.h"
#include "define.h"
#include <chrono>
#include <thread>

class SensorHandler
{
public:
    SensorHandler();
    ~SensorHandler();
private:
    void generateSignal1();
    void generateSignal2();
private:
    const int SAMPLERATE1 = 1000;
    const int SAMPLERATE2 = 1200;
    std::shared_ptr<PackageEncapsulation> mPackageEncap;
    std::shared_ptr<BluezSocket> mSenderSocket;
    uint16_t mSeqNumber1{0};
    uint16_t mSeqNumber2{0};
    uint16_t mAckNumber1{0};
    uint16_t mAckNumber2{0};
    std::thread tGenerateSignal1;
    std::thread tGenerateSignal2;
};

#endif // SENSORHANDLER_H
