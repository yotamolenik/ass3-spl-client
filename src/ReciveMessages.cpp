//
// Created by olenikyo@wincs.cs.bgu.ac.il on 12/28/18.
//

#include "ReciveMessages.h"
#include "../include/connectionHandler.h"
#include <mutex>


//t constructor locks the mutex while destructor (out of scope) unlocks it   //t std::lock_guard<std::mutex> lock(_mutex);

ReciveMessages::ReciveMessages(ConnectionHandler &connectionHandler, std::mutex &mutex) :_connectionHandler(connectionHandler), _mutex(mutex){}

void ReciveMessages::run() {
    while(1){
        std::string answer;
        //here we get the line from the server. answer becomes ascii
        if (!_connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        //TODO: i need encoderDecoder but what do i do with it?
//        len = answer.length();
//        Message message = encodec.decodeNextByte;
//        if (message != null){
//            process(message);
//        }
    }
}

