#include "CachingPurposeVector.h"

CachingPurposeVector::CachingPurposeVector(int capacity, int windowSize, int boundary)
{
    mCapacity = capacity;
    mWindowSize = windowSize;
    mBoundary = boundary;
    mData = std::vector<std::unique_ptr<Message>>(mCapacity);
}

void CachingPurposeVector::insertMessage(std::unique_ptr<Message> msg)
{
    std::unique_lock<std::mutex> lk(mMutex);
    if(!isFirstTimeInserted){
        mLatestProcessIndex = msg->seqNumber() % mCapacity;
        isFirstTimeInserted = true;
    }
    mData[msg->seqNumber() % mCapacity] = std::move(msg);
    ++mSize;
    if(mSize >= mBoundary) mCV.notify_one();
}

std::vector<std::unique_ptr<Message> > CachingPurposeVector::getListMessage()
{
    std::vector<std::unique_ptr<Message>> res;
    std::unique_lock<std::mutex> lk(mMutex);
    if(mSize < mBoundary){
        auto waitSucess = mCV.wait_for(lk, std::chrono::milliseconds(2000), [this]{return mSize >= mBoundary;});
        if(!waitSucess){
            std::cout<<"take all data that cache had";
            res.reserve(mSize);
            for(int i = 0; i < mSize; i++){
                res.push_back(std::move(mData[(mLatestProcessIndex + i) % mCapacity]));
            }
            mLatestProcessIndex = (mLatestProcessIndex + mSize) % mCapacity;
            mSize = 0;
        }
        else{
            res.reserve(mWindowSize);
            for(int i = 0; i < mWindowSize; i++){
                res.push_back(std::move(mData[(mLatestProcessIndex + i) % mCapacity]));
            }
            mLatestProcessIndex = (mLatestProcessIndex + mWindowSize) % mCapacity;
            mSize -= mWindowSize;
        }
    }
    else{
        res.reserve(mWindowSize);
        for(int i = 0; i < mWindowSize; i++){
            res.push_back(std::move(mData[(mLatestProcessIndex + i) % mCapacity]));
        }
        mLatestProcessIndex = (mLatestProcessIndex + mWindowSize) % mCapacity;
        mSize -= mWindowSize;
    }
    return res;
}
