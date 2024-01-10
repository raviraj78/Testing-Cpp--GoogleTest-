#ifndef SERVER_H
#define SERVER_H
 
class Server {
public:
    // Constructor
    Server(int serverQueueKey);
 
    // Function to start listening for messages
    void startListening();
 
private:
    int serverQueueId; // Server's message queue ID
};
 
#endif // SERVER_H