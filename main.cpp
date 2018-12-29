#include <iostream>
#include <stdlib.h>
#include "include/connectionHandler.h"
#include "src/ReceiveMessages.h"
#include "include/Message.h"
#include <mutex>
#include <thread>

void shortToBytes(short opcode, int i);

using namespace std;
//TODO: fix the build, something with boost

// is for my comments
//t is for the original comments

//my main is in charge of sending messages and one more thread is in charge of receiving messages



//t This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    std::mutex mutex;
    ConnectionHandler connectionHandler(host, port);
    ReceiveMessages receiveMessages(connectionHandler, mutex);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    std::thread th1(&ReceiveMessages::run, &receiveMessages);
    th1.join();

    while (1) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();
        //first receive message, then parse according to the assignment, then send the result
        //parse the command from the keyboard
        std::string firstWord = line.substr(0, line.find(' '));
        if (firstWord == "REGISTER"){
            short opcode = 1;
            char bytes[2];
            shortToBytes(opcode,*bytes);
            string afterFirstSpace = line.substr(line.find(' '));
            string username = afterFirstSpace.substr(0,afterFirstSpace.find(' '));
            string password = afterFirstSpace.substr(afterFirstSpace.find(' '));
            string toSend = std::to_string(opcode) + username + "\0" + password + "\0";
            connectionHandler.sendLine(toSend);
        }
        if (firstWord == "LOGIN"){

        }
        if (firstWord == "LOGOUT"){

        }
        if (firstWord == "FOLLOW"){

        }
        if (firstWord == "POST"){

        }
        if (firstWord == "PM"){

        }
        if (firstWord == "USERLIST"){

        }
        if (firstWord == "STAT"){

        }

        //send to the server the command from the keyboard
        if (!connectionHandler.sendLine(line)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        //t connectionHandler.sendLine(line) appends '\0' to the message. Therefor we send len+1 bytes.

        //t We can use one of three options to read data from the server:
        //t 1. Read a fixed number of characters
        //t 2. Read a line (up to the newline character using the getline() buffered reader
        //t 3. Read up to the null character

        //t Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        //t We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
    }
    return 0;
}

void shortToBytes(short opcode, int i) {

}

void shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}



