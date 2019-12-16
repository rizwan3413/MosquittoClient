#include <iostream>
#include "DataObserver.h"
#include <signal.h>

void signalHandler(int signum)
{
    switch (signum)
    {
        case SIGINT:
            if(DataObserver::getClientDataPointer()) {
                delete DataObserver::getClientDataPointer();
            }
            break;
        case SIGTERM:
            if (DataObserver::getClientDataPointer()) {
                delete DataObserver::getClientDataPointer();
            }
            break;
    }
}

int main() {
    std::cout << "Starting DataObserver client to receive and send BLE, GNSS data\n";
    DataObserver::startDataObserver("DATA", "localhost", 1883,60);
    while(true) {}
    return 0;
}
