#include "SignalProcessPurposeVector.h"

SignalProcessPurposeVector::SignalProcessPurposeVector(int capacity, int batchSize)
{
    mBatchSize = batchSize;
    if(capacity >= 3*mBatchSize) mCapacity = capacity;
    else mCapacity = 3*mBatchSize;
    mData = std::vector<char>(mCapacity);
}

std::vector<char> &SignalProcessPurposeVector::data()
{
    return mData;
}

void SignalProcessPurposeVector::insertData(const std::vector<char> &insertedData)
{
    std::unique_lock<std::mutex> lk(mMutex);
    for(int i = 0; i < insertedData.size(); i++){
        mData[(mCurrentIndex + i) % mCapacity] = insertedData[i];
    }
    mCurrentIndex = (mCurrentIndex + insertedData.size()) % mCapacity;
    mSize += insertedData.size();
}

std::vector<char> SignalProcessPurposeVector::getProcessData()
{
    int len=0;
    std::vector<char> res;
    std::unique_lock<std::mutex> lk(mMutex);
    int size = mSize;
    lk.unlock();

    if(size > 0){
        if(size > mBatchSize){
            len = mBatchSize;
        }
        else{
            len = size;
        }
        res.reserve(len);
        for(int i = 0; i < len; i++){
            res.push_back(mData[(mLatestProcessIndex+i)%mCapacity]);
        }
        mLatestProcessIndex = (mLatestProcessIndex + len) % mCapacity;
        lk.lock();
        mSize -= len;
        lk.unlock();
    }
    return res;
}
