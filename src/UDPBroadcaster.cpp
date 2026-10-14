#include "UDPBroadcaster.h"
#include <cstring>
#include <unistd.h>

UDPBroadcaster::UDPBroadcaster(const std::string& ip, const int port)
    : mIp(ip), mPort(port), mSockFileDescriptor(socket(AF_INET, SOCK_DGRAM, 0))
{
    if (mSockFileDescriptor == -1)
    {
        throw std::runtime_error("Socket creation failed");
    }

    memset(&mServerAddress, 0, sizeof(mServerAddress));
    mServerAddress.sin_family = AF_INET;
    mServerAddress.sin_port = htons(mPort);
    if (inet_pton(AF_INET, mIp.c_str(), &mServerAddress.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address / Address not supported");
    }
}

UDPBroadcaster::~UDPBroadcaster()
{
    close(mSockFileDescriptor);
}

void UDPBroadcaster::send(const std::string& msg)
{
    sendto(mSockFileDescriptor, msg.c_str(), msg.length(), 0, reinterpret_cast<struct sockaddr*>(&mServerAddress),
           sizeof(mServerAddress));
}
