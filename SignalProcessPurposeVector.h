#ifndef SIGNALPROCESSPURPOSEVECTOR_H
#define SIGNALPROCESSPURPOSEVECTOR_H

#include <vector>
#include <mutex>
class SignalProcessPurposeVector
{
public:
    SignalProcessPurposeVector(int capacity, int batchSize);
    std::vector<char>& data();
    void insertData(const std::vector<char>& insertedData);
    std::vector<char> getProcessData();
private:
    int mCapacity;
    int mSize{0};
    std::vector<char> mData;
    int mLatestProcessIndex{0};
    int mBatchSize;
    int mCurrentIndex{0};
    std::mutex mMutex;
};

#endif // SIGNALPROCESSPURPOSEVECTOR_H
