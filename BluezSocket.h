#ifndef BLUEZSOCKET_H
#define BLUEZSOCKET_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <vector>

#define BUFFER_SIZE 1024
class BluezSocket
{
public:
    BluezSocket();
    int open();
    int close();
    int connect();
    std::vector<char> read();
    int write(const std::vector<char>& data);
private:
    int mSocket;
    struct sockaddr_l2 mAddr;
    char mBuffer[BUFFER_SIZE];
};

#endif // BLUEZSOCKET_H
