#include <iostream>
#include <stdlib.h>
#include "include/connectionHandler.h"
#include "src/ReceiveMessages.h"
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

// is for my comments
//t is for the original comments

//my main is in charge of sending messages and one more thread is in charge of receiving messages



//t This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
void shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}


string prepareLoginOrRegister(short op, string &line, ConnectionHandler &connectionHandler) {
    line[line.find(' ')] = '\0';
    char toSend[2];
    shortToBytes(op,toSend);
    for (int i = 0; i < line.size(); ++i) {
        toSend[i+2] = line[i];
    }
    toSend[line.size() + 3] = '\0';
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
    terminate.store(false);
    ReceiveMessages receiveMessages(connectionHandler, mutex, terminate);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    std::thread th1(&ReceiveMessages::run, &receiveMessages);

    while (!terminate.load()) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        //int len = (int)line.length(); that line was here but im not sure why
        //first receive message, then parse according to the assignment, then send the result
        std::string firstWord = line.substr(0, line.find(' '));
        if (firstWord == "REGISTER") {
            string prepare = prepareLoginOrRegister(1, line, connectionHandler);
            // convert the string back to an array because sendBytes is stupid
            char * tosend;
            for (int i = 0; i < prepare.size(); ++i) {
                tosend[i] = prepare[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)prepare.size())) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "LOGIN") {
            string prepare = prepareLoginOrRegister(2, line, connectionHandler);
            char * tosend;
            for (int i = 0; i < prepare.size(); ++i) {
                tosend[i] = prepare[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)prepare.size())) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

        }
        if (firstWord == "LOGOUT") {
            short op = 3;
            char opcode[2];
            shortToBytes(op, opcode);
            if (!connectionHandler.sendBytes(opcode,2)) {
                std::cout << "already logged out\n" << std::endl;
                break;
            }
            break;
        }
        if (firstWord == "FOLLOW") {
            int foun = std::stoi(line.substr(8,9));
            short numOfUsers = (short)std::stoi(line.substr(11,12));
            for (char &c : line) {
                if(c == ' ')
                    c = '\0';
            }
            short op = 4;
            char opcode[2];
            shortToBytes(op,opcode);
            char * tosend;
            tosend[0] = opcode[0];
            tosend[1] = opcode[1];
            tosend[2] = (char)foun;
            char numberOfUsers[2];
            shortToBytes(numOfUsers,numberOfUsers);
            tosend[3] = numberOfUsers[0];
            tosend[4] = numberOfUsers[1];
            for (int i = 0; i < line.size(); ++i) {
                tosend[i+5] = line[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)line.size() + 5)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "POST") {
            line.append("\0");
            short op = 5;
            char opcode[2];
            shortToBytes(op,opcode);
            char * tosend;
            tosend[0] = opcode[0];
            tosend[1] = opcode[1];
            for (int i = 0; i < line.size(); ++i) {
                tosend[i+2] = line[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)line.size())) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "PM") {
            short op = 6;
            char opcode[2];
            shortToBytes(op,opcode);
            line[line.find(' ')] = '\0';
            line.append("\0");
            char * tosend;
            for (int i = 0; i < line.size(); ++i) {
                tosend[i+2] = line[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)line.size())) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "USERLIST") {
            short op = 7;
            char opcode[2];
            shortToBytes(op,opcode);
            if (!connectionHandler.sendBytes(opcode,2)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if (firstWord == "STAT") {
            line.append("\0");
            short op = 8;
            char opcode[2];
            shortToBytes(op,opcode);
            char * tosend;
            tosend[0] = opcode[0];
            tosend[1] = opcode[1];
            for (int i = 0; i < line.size(); ++i) {
                tosend[i] = line[i];
            }
            if (!connectionHandler.sendBytes(tosend,(int)line.size())) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
    }
    th1.join();
    return 0;
}


//std::vector<char> mergeArrIntoVector(string op, string username, string password){
//    vector<char> merged;
//    merged.emplace_back(op);
//    merged.emplace_back(username);
//    merged.emplace_back(password);
//    return merged;
//}

//t connectionHandler.sendLine(line) appends '\0' to the message. Therefor we send len+1 bytes.

//t We can use one of three options to read data from the server:
//t 1. Read a fixed number of characters
//t 2. Read a line (up to the newline character using the getline() buffered reader
//t 3. Read up to the null character

//t Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
//t We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end

