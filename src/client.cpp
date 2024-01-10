#include "client.h"
#include <iostream>
#include <cstring>
#include <sys/msg.h>
 
// Define a structure for the message
struct Message {
    long messageType;
    char messageText[256];
};
 
Client::Client(const std::string& u, int serverQueueKey) : username(u){
    // Get the server's message queue ID
}

void Client::connectToServer() {
        std::string connectMessage = "connect "+username;
        sendMessage(connectMessage);
    }
 
void Client::sendMessage(const std::string& message) {
    Message msg;
    msg.messageType = 1; // Message type 1 for sending messages
    std::strcpy(msg.messageText, (username + ": " + message).c_str());
 
    // Send the message to the server
    msgsnd(serverQueueId, &msg, sizeof(msg.messageText), 0);
}
 
std::string Client::receiveMessage() {
    Message msg;
    // Receive the message from the server
    if (msgrcv(serverQueueId, &msg, sizeof(msg.messageText), 2, IPC_NOWAIT) != -1) {
        // Message type 2 for receiving messages
        return std::string(msg.messageText);
    }
    return ""; // No message received
}