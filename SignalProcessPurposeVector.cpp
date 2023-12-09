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

SignalProcessPurposeVector::ProcessIndexs SignalProcessPurposeVector::getProcessIndexs()
{
    std::unique_lock<std::mutex> lk(mMutex);
    ProcessIndexs processIndexs;
    if(mSize > 0){
        processIndexs.startIndex = mLatestProcessIndex;
        if(mSize > mBatchSize){
            processIndexs.length = mBatchSize;
            mSize -= mBatchSize;
        }
        else{
            processIndexs.length = mSize;
            mSize = 0;
        }
        mLatestProcessIndex = (mLatestProcessIndex + processIndexs.length) % mCapacity;
    }
    return processIndexs;
}
