#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class UDPListener
{
public:
    explicit UDPListener(int port);
    ~UDPListener();

    void setOnDataReceivedCallback(const std::function<void(const std::string&)>& callback);
    void startListening();
    void stopListening();

private:
    int port;
    std::thread listenerThread;
    std::function<void(const std::string&)> onDataReceivedCallback;
    std::atomic<bool> stopFlag;
    std::mutex mutex;

    void listen() const;
};
