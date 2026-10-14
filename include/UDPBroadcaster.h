#pragma once

#include <iostream>
#include <arpa/inet.h>

class UDPBroadcaster
{
public:
    UDPBroadcaster(const std::string& ip, int port);
    ~UDPBroadcaster();

    void send(const std::string& msg);

private:
    std::string mIp;
    int mPort;
    int mSockFileDescriptor;
    sockaddr_in mServerAddress;
};
