#ifndef PHONERECEIVER_H
#define PHONERECEIVER_H

#include <define.h>
#include <thread>
#include <queue>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "PackageEncapsulation.h"
#include "SignalProcessPurposeVector.h"
#include "CachingPurposeVector.h"

class PhoneReceiver
{
public:
    PhoneReceiver();
    ~PhoneReceiver();
private:
    void receiveData();
    void processData();
    void handleMsgQueue1();
    void handleMsgQueue2();
    void processSignal();
    void executeAlgorithm(char *dataSignal1, int lenData1, char* dataSignal2, int lenData2);
private:
    const int SampleRate1 = 1000; // 10000 samples per second
    const int SampleRate2 = 1200; // 12000 samples per second
    int mSocket;
    std::queue<std::vector<char>> mReceivedQueue;
    std::shared_ptr<CachingPurposeVector> mCachingMsg1;
    std::shared_ptr<CachingPurposeVector> mCachingMsg2;
    std::shared_ptr<SignalProcessPurposeVector> mSignalProcessVector1;
    std::shared_ptr<SignalProcessPurposeVector> mSignalProcessVector2;
    std::shared_ptr<PackageEncapsulation> mPackageEncap;
    std::mutex mReceivedQueueMutex;
    std::condition_variable mReceivedQueueEmptyCV;
    std::thread tReceiveData;
    std::thread tProcessData;
    std::thread tHandleMsgQueue1;
    std::thread tHandleMsgQueue2;
    std::thread tProcessSignal;
};

#endif // PHONERECEIVER_H
