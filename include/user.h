#ifndef USER_H
#define USER_H
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<ctime>
#include<algorithm>
#include<cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "client.h"
#include "server.h"
#include<thread>
#include<unistd.h>


class User {
public:
    std::string name;
    std::string username;
    std::string password;
    std::string contact;
 
    User(const std::string& n, const std::string& u, const std::string& p, const std::string& c)
        : name(n), username(u), password(p), contact(c) {}
                       
    std::string getName() const;
    std::string getUserName() const;
    std::string getPassword() const;
    std::string getContact() const;
    static std::vector<User> readDataFromFile(const std::string& filename);
    static void writeDataToFile(const std::string& filename, const std::vector<User>& users);
 
    static bool isUsernameTaken(const std::string& username, const std::vector<User>& users);
    static bool loginUser(const std::string& username, const std::string& password, const std::vector<User>& users);
    static void registerUser(const std::string& filename);
    static void scheduleAppointment(const std::string& providerUsername, const std::string& time, const std::string& inputUsername, const std::string& filename, const std::string& appointmentsFilename);
    static bool isAppointmentSlotAvailable(const std::string& providerUsername, const std::string& time, const std::string& appointmentsFilename);
    static void viewAppointments(const std::string& name, const std::string& appointmentsFilename);
    static void sendMsg(const std::string& doctorUsername, const std::string& username);
};
#endif