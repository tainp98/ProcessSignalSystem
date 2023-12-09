#include "PhoneReceiver.h"

PhoneReceiver::PhoneReceiver()
{
    mCachingMsg1 = std::make_shared<CachingPurposeVector>(1000, 10, 50);
    mCachingMsg2 = std::make_shared<CachingPurposeVector>(1000, 10, 50);
    mPackageEncap = std::make_shared<PackageEncapsulation>();
    mSignalProcessVector1 = std::make_shared<SignalProcessPurposeVector>(3*SampleRate1*10, SampleRate1*10);
    mSignalProcessVector2 = std::make_shared<SignalProcessPurposeVector>(3*SampleRate2*10, SampleRate2*10);

    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    mSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    // bind socket to port 0x1001 of the first available
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = {{0, 0, 0, 0, 0, 0}};
    loc_addr.l2_psm = htobs(0x1001);
    bind(mSocket, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    tReceiveData = std::thread(&PhoneReceiver::receiveData, this);
    tProcessData = std::thread(&PhoneReceiver::processData, this);
    tHandleMsgQueue1 = std::thread(&PhoneReceiver::handleMsgQueue1, this);
    tHandleMsgQueue2 = std::thread(&PhoneReceiver::handleMsgQueue2, this);
    tProcessSignal = std::thread(&PhoneReceiver::processSignal, this);
}

PhoneReceiver::~PhoneReceiver()
{
    if(tReceiveData.joinable()) tReceiveData.join();
    if(tProcessData.joinable()) tProcessData.join();
    if(tHandleMsgQueue1.joinable()) tHandleMsgQueue1.join();
    if(tHandleMsgQueue2.joinable()) tHandleMsgQueue2.join();
    if(tProcessSignal.joinable()) tProcessSignal.join();
}

void PhoneReceiver::receiveData()
{
    listen(mSocket, 1);
    // accept one connection
    struct sockaddr_l2 rem_addr = { 0 };
    socklen_t opt = sizeof(rem_addr);
    int client = accept(mSocket, (struct sockaddr *)&rem_addr, &opt);
    char buf[1024] = { 0 };
//    ba2str( &rem_addr.l2_bdaddr, buf );
//    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    int bytes_read;
    while (true) {
        // read data from socket
        // push number of bytes data into queue, and then parsing to message
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            printf("received [%s]\n", buf);
            std::vector<char> data(bytes_read);
            for(int i = 0; i < bytes_read; i++){
                data[i] = buf[i];
            }
            std::unique_lock<std::mutex> lk(mReceivedQueueMutex);
            mReceivedQueue.push(std::move(data));
            mReceivedQueueEmptyCV.notify_one();
        }

    }
}

void PhoneReceiver::processData()
{
    while (true) {
        std::unique_ptr<Message> msg;
        std::unique_lock<std::mutex> lk(mReceivedQueueMutex);
        if(mReceivedQueue.size() == 0) mReceivedQueueEmptyCV.wait(lk, [this]{ return mReceivedQueue.size() > 0;});
        msg = mPackageEncap->toMessage(mReceivedQueue.front());
        mReceivedQueue.pop();
        lk.unlock();
        if(!msg) continue;
        if(msg->sensorType() == SensorType::SENSOR1){
            mCachingMsg1->insertMessage(std::move(msg));
        }
        else if(msg->sensorType() == SensorType::SENSOR2){
            mCachingMsg2->insertMessage(std::move(msg));
        }
    }
}

void PhoneReceiver::handleMsgQueue1()
{
    while (true) {
        auto msgVec = mCachingMsg1->getListMessage();
        for(int i = 0; i < msgVec.size(); i++){
            mSignalProcessVector1->insertData(msgVec[i]->payload());
        }
    }
}

void PhoneReceiver::handleMsgQueue2()
{
    while (true) {
        auto msgVec = mCachingMsg2->getListMessage();
        for(int i = 0; i < msgVec.size(); i++){
            mSignalProcessVector2->insertData(msgVec[i]->payload());
        }
    }
}

// this function process data from sensor each 10s
void PhoneReceiver::processSignal()
{
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        auto processIndexs1 = mSignalProcessVector1->getProcessIndexs();
        auto processIndexs2 = mSignalProcessVector2->getProcessIndexs();
        executeAlgorithm(mSignalProcessVector1->data().data() + processIndexs1.startIndex, processIndexs1.length,
                         mSignalProcessVector2->data().data() + processIndexs2.startIndex, processIndexs2.length);
        auto stop = std::chrono::high_resolution_clock::now();
        auto exTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000) - exTime);
    }
}

void PhoneReceiver::executeAlgorithm(char *dataSignal1, int lenData1, char* dataSignal2, int lenData2)
{
    // todo
}


