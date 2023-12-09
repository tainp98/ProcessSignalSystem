#ifndef SIGNALPROCESSPURPOSEVECTOR_H
#define SIGNALPROCESSPURPOSEVECTOR_H

#include <vector>
#include <mutex>
class SignalProcessPurposeVector
{
public:
    struct ProcessIndexs{
        int startIndex{-1};
        int length{-1};
    };
    SignalProcessPurposeVector(int capacity, int batchSize);
    std::vector<char>& data();
    void insertData(const std::vector<char>& insertedData);
    ProcessIndexs getProcessIndexs();
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
