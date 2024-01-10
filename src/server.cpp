#include "server.h"
#include <iostream>
#include <cstring>
#include <sys/msg.h>
 
// Define a structure for the message
struct Message {
    long messageType;
    char messageText[256];
};
 
Server::Server(int serverQueueKey) {
    // Create or get the server's message queue ID
    serverQueueId = msgget(serverQueueKey, 0666 | IPC_CREAT);
    if (serverQueueId == -1) {
        std::cerr << "Error: Unable to create or connect to the server's message queue.\n";
        // Handle the error appropriately in your application
    }
}
 
void Server::startListening() {
    while (true) {
        Message msg;
        // Receive the message from any client (message type 1)
        if (msgrcv(serverQueueId, &msg, sizeof(msg.messageText), 1, 0) != -1) {
            // For simplicity, just print the received message
            std::cout << "Received message: " << msg.messageText << "\n";
 
            // Assuming you have logic to check doctor's status (online or offline)
            // If online, send a response back to the client (message type 2)
            if (true /* Doctor is online */) {
                msg.messageType = 2;
                std::strcpy(msg.messageText, "Doctor is online");
                msgsnd(serverQueueId, &msg, sizeof(msg.messageText), 0);
            } else {
                std::cout << "Doctor is offline\n";
            }
        }
 
        // Add other logic as needed
    }
}