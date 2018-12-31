#include <iostream>
#include <stdlib.h>
#include "include/connectionHandler.h"
#include "src/ReceiveMessages.h"
#include <mutex>
#include <thread>

using namespace std;
//TODO: fix the build, something with boost

// is for my comments
//t is for the original comments

//my main is in charge of sending messages and one more thread is in charge of receiving messages



//t This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
void shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

string prepareLoginOrRegister(short op, string line, ConnectionHandler &connectionHandler) {
    short opcode = op;
    char bytes[2];
    shortToBytes(opcode, bytes);
    string afterFirstSpace = line.substr(line.find(' '));
    string username = afterFirstSpace.substr(0, afterFirstSpace.find(' '));
    string password = afterFirstSpace.substr(afterFirstSpace.find(' '));
    string toSend = std::to_string(bytes) + username + "\0" + password + "\0";
    return toSend;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    std::mutex mutex;
    ConnectionHandler connectionHandler(host, port);
    std::atomic<bool> terminate{};
    terminate.store(true);
    ReceiveMessages receiveMessages(connectionHandler, mutex, terminate);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    std::thread th1(&ReceiveMessages::run, &receiveMessages);
    th1.join();

    while (terminate.load()) {// after th1 is constructed it is joinable. after destruction, not anymore
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();
        //first receive message, then parse according to the assignment, then send the result
        std::string firstWord = line.substr(0, line.find(' '));
        if (firstWord == "REGISTER") {
            string tosend = prepareLoginOrRegister(1, line, connectionHandler);
            if (!connectionHandler.sendLine(tosend)) { //after logoutACK the socket is closed so sendline will fail
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

        }
        if (firstWord == "LOGIN") {
            string tosend = prepareLoginOrRegister(2, line, connectionHandler);
            if (!connectionHandler.sendLine(tosend)) { //after logoutACK the socket is closed so sendline will fail
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

        }
        if (firstWord == "LOGOUT") {
            short opcode = 3;
            char bytes[2];
            shortToBytes(opcode, bytes);
            if (!connectionHandler.sendBytes(bytes)) { //after logoutACK the socket is closed so sendline will fail
                std::cout << "already logged out\n" << std::endl;
                break;
            }
            connectionHandler.sendBytes(bytes, 2);
        }
        if (firstWord == "FOLLOW") {
            int foun = std::stoi(line.substr(0,1));
            int numOfUsers = std::stoi(line.substr(1,2));
            for (int i = 0; i < line.size(); i++){
                if(line[i] == ' ')
                  line[i] = '\0';
            }
            short opcode = 4;
            char bytes[2];
            shortToBytes(opcode, bytes);

            if (!connectionHandler.sendLine(line)) { //after logoutACK the socket is closed so sendline will fail
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "POST") {

        }
        if (firstWord == "PM") {

        }
        if (firstWord == "USERLIST") {

        }
        if (firstWord == "STAT") {

        }
    }
    return 0;
}




//t connectionHandler.sendLine(line) appends '\0' to the message. Therefor we send len+1 bytes.

//t We can use one of three options to read data from the server:
//t 1. Read a fixed number of characters
//t 2. Read a line (up to the newline character using the getline() buffered reader
//t 3. Read up to the null character

//t Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
//t We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end

