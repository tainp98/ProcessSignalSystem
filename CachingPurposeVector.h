#ifndef CACHINGPURPOSEVECTOR_H
#define CACHINGPURPOSEVECTOR_H
#include <vector>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Message.h"

class CachingPurposeVector
{
public:
    CachingPurposeVector(int capacity, int windowSize, int boundary);
    void insertMessage(std::unique_ptr<Message> msg);
    std::vector<std::unique_ptr<Message>> getListMessage();

private:
    int mCapacity;
    int mSize{0};
    int mLatestProcessIndex{0};
    int mWindowSize{1};
    int mBoundary{50};
    bool isFirstTimeInserted{false};
    std::vector<std::unique_ptr<Message>> mData;
    std::mutex mMutex;
    std::condition_variable mCV;
};

#endif // CACHINGPURPOSEVECTOR_H
