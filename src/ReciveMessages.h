//
// Created by olenikyo@wincs.cs.bgu.ac.il on 12/28/18.
//

#ifndef CLIENT_RECIVEMESSAGES_H
#define CLIENT_RECIVEMESSAGES_H


#include <mutex>
#include "../include/connectionHandler.h"

class ReciveMessages {
private:
    ConnectionHandler _connectionHandler;
    std::mutex &_mutex;
public:
    ReciveMessages(ConnectionHandler &connectionHandler, std::mutex &mutex);
    void run();
};



#endif //CLIENT_RECIVEMESSAGES_H
