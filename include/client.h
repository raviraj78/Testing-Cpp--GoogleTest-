#ifndef CLIENT_H
#define CLIENT_H
 
#include <string>
 
class Client {
public:
    // Constructor
    Client(const std::string& username, int serverQueueKey);

    void connectToServer();
 
    // Function to send a message to the server
    void sendMessage(const std::string& message);
 
    // Function to receive messages from the server
    std::string receiveMessage();
 
private:
    std::string username; // Client's username
    int serverQueueId;    // Server's message queue ID
};
 
#endif // CLIENT_H