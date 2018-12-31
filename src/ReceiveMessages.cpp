#include "ReceiveMessages.h"
#include "../include/connectionHandler.h"
#include <mutex>
using namespace std;



//t constructor locks the mutex while destructor (out of scope) unlocks it   //t std::lock_guard<std::mutex> lock(_mutex);

ReceiveMessages::ReceiveMessages(ConnectionHandler &connectionHandler, std::mutex &mutex,
                                 std::atomic<bool> &terminate) : ch(connectionHandler), _mutex(mutex), _terminate(terminate) {

}

void ReceiveMessages::run() {
    char answer [2];
    while (_terminate.load()) {
        ch.getBytes(answer , 2);
        process(answer);
    }
}



void ReceiveMessages::process(char *ans){
    int opCode = bytesToShort(ans);
    switch (opCode) {
        case 9 : {
            string opCodeString = "NOTIFICATION ";
            char NotificationType[1];
            ch.getBytes(NotificationType, 1);
            int numOfNotification = bytesToShort(NotificationType);
            string NotificationToPrint;
            if (numOfNotification == 0)
                NotificationToPrint = "PM";
            else NotificationToPrint = "Public";
            string PostingUser;
            ch.getLine(PostingUser);
            PostingUser = PostingUser.substr(3);// delete the opCode and NotificationType
            string content;
            ch.getLine(content);

            cout << opCodeString + " " + NotificationToPrint + " " + PostingUser + " " + content<< endl;
            break;
        }

        case 10: {
            string opCodeString = "ACK";
            char MessageOpCode[2];
            ch.getBytes(MessageOpCode, 2);
            int MessageOP = bytesToShort(MessageOpCode);
            switch (MessageOP) {
                case 3: {
                    ch.close();
                    _terminate.store(false);
                }
                case 4: {
                    char NumOfUsers [2];
                    ch.getBytes(NumOfUsers , 2);
                    int NumUsers = bytesToShort(NumOfUsers);
                    string UserNameList;
                    for(unsigned int i = 0 ; i < NumUsers ; i++ ){
                        string userName;
                        ch.getLine(userName);
                        UserNameList = UserNameList + userName +" ";

                    }
                    cout << opCodeString + " " + std::to_string(MessageOP) + " " + NumOfUsers + " " + UserNameList<< endl;
                }
                case 7 : {
                    string Users;
                    ch.getLine(Users);
                    string NumOfUsers = Users.substr(0, 2);
                    string UserNameList = Users.substr(2);
                    cout << opCodeString + " " + std::to_string(MessageOP) + " " + NumOfUsers + " " + UserNameList<< endl;
                }
                case 8 : {
                    string User;
                    ch.getLine(User);
                    string NumPosts = User.substr(0, 2);
                    string NumFollowers = User.substr(2, 4);
                    string NumFollowing = User.substr(4);
                    cout << opCodeString + " " + std::to_string(MessageOP) + " " + NumPosts + " " + NumFollowers + " " +
                            NumFollowing << endl;
                }
            }
        }
        case 11 : {
            char MessageOpCode[2];
            ch.getBytes(MessageOpCode, 2);
            int MessageOP = bytesToShort(MessageOpCode);
            cout<< "Error " + std::to_string(MessageOP) << endl;
        }
    }
}

short ReceiveMessages::bytesToShort(char *bytesArr) {
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}








