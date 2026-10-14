#include "UDPListener.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

UDPListener::UDPListener(int port)
    : port(port), onDataReceivedCallback(nullptr), stopFlag(false)
{
    listenerThread = std::thread(&UDPListener::listen, this);
}

UDPListener::~UDPListener()
{
    stopListening();
    if (listenerThread.joinable())
    {
        listenerThread.join();
    }
}

void UDPListener::setOnDataReceivedCallback(const std::function<void(const std::string&)>& callback)
{
    onDataReceivedCallback = callback;
}

void UDPListener::startListening()
{
    // This method is a placeholder for starting the listener in a separate thread
    listenerThread = std::thread(&UDPListener::listen, this);
}

void UDPListener::stopListening()
{
    stopFlag = true;
}

void UDPListener::listen() const
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        throw std::runtime_error("Error creating socket");
    }

    sockaddr_in serverAddress = {}, clientAddress = {};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(sockfd, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0)
    {
        close(sockfd);
        throw std::runtime_error("Error binding socket");
    }

    char buffer[1024];
    socklen_t len = sizeof(clientAddress);

    pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    while (!stopFlag)
    {
        int poll_result = poll(fds, 1, 500); // 0.1 second timeout
        if (poll_result < 0)
        {
            if (errno == EINTR)
            {
                continue; // Interrupted by signal, try again
            }
            close(sockfd);
            throw std::runtime_error("Error in poll");
        }
        if (poll_result == 0)
        {
            // No data available, continue to the next iteration
            continue;
        }

        if (fds[0].revents & POLLIN)
        {
            const ssize_t n = recvfrom(sockfd, (char*)buffer, 1024, 0, reinterpret_cast<struct sockaddr*>(&clientAddress), &len);
            if (n < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // No data available, continue to the next iteration
                    continue;
                }
                close(sockfd);
                throw std::runtime_error("Error receiving data");
            }

            buffer[n] = '\0';
            if (onDataReceivedCallback)
            {
                onDataReceivedCallback(std::string(buffer, n));
            }
        }
    }

    close(sockfd);
}
