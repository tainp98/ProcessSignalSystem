#include "BluezSocket.h"

BluezSocket::BluezSocket()
{

}

int BluezSocket::open()
{
    mSocket = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    mAddr.l2_family = AF_BLUETOOTH;
    mAddr.l2_bdaddr = {{0, 0, 0, 0, 0, 0}};
    mAddr.l2_psm = htobs(0x1001);
    return mSocket;
}

int BluezSocket::close()
{
    return ::close(mSocket);
}

int BluezSocket::connect()
{
    int status = ::connect(mSocket, (struct sockaddr *)&mAddr, sizeof(mAddr));
    std::cout << "status = "<< status << "\n";
    return status;
}

std::vector<char> BluezSocket::read()
{
    std::vector<char> res;
    int bytes_read = ::read(mSocket, mBuffer, BUFFER_SIZE);
    res.reserve(bytes_read);
    for(int i = 0; i < bytes_read; i++){
        res[i] = mBuffer[i];
    }
    return res;
}

int BluezSocket::write(const std::vector<char> &data)
{
    return ::write(mSocket, data.data(), data.size());
}
