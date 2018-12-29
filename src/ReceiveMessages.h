
#ifndef CLIENT_RECIVEMESSAGES_H
#define CLIENT_RECIVEMESSAGES_H


#include <mutex>
#include "../include/connectionHandler.h"

class ReceiveMessages {
private:
    ConnectionHandler &ch;
    std::mutex &_mutex;
public:
    ReceiveMessages(ConnectionHandler &connectionHandler, std::mutex &mutex);
    void run();
    void process(char *ans);
    short bytesToShort(char* bytesArr);
private:
    bool terminate;
};



#endif //CLIENT_RECIVEMESSAGES_H
