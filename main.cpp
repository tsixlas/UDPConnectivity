#include "UDPBroadcaster.h"
#include "UDPListener.h"
#include <unistd.h>
#include <functional>
#include <iostream>

int main() {
    try {
        for (int i=0;i<2;i++)
        {
            // Create the listener
            UDPListener listener(12345);
            listener.setOnDataReceivedCallback([](const std::string& data) {
                std::cout << "Received: " << data << std::endl;
            });

            // Start the broadcaster
            UDPBroadcaster broadcaster("127.0.0.1", 12345);
            for (int i = 0; i < 10; ++i) {
                broadcaster.send("Hello, UDP Listener! " + std::to_string(i));
                usleep(10000);
            }

            // Stop the listener
            listener.stopListening();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}