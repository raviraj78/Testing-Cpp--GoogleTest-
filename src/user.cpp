#include"user.h"
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

std::string User::getName() const {
    return name;
}

std::string User::getUserName() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getContact() const {
    return contact;
}

std::vector<User> User::readDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<User> users;
    std::string name, username, password, contact;
 
    while (file >> name >> username >> password >> contact) {
        users.emplace_back(name, username, password, contact);
    }
  
    file.close();
    return users;
}
 
void User::writeDataToFile(const std::string& filename, const std::vector<User>& users) {
    std::ofstream file(filename);
 
    for (const auto& user : users) {
file << user.name << " " << user.username << " " << user.password << " " << user.contact << "\n";
    }
 
    file.close();
}
 
bool User::isUsernameTaken(const std::string& username, const std::vector<User>& users) {
    for (const auto& user : users) {
        if (user.username == username) {
            return true; // Username already taken
        }
    }
    return false; // Username is available
}
 
bool User::loginUser(const std::string& username, const std::string& password, const std::vector<User>& users) {
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            return true; // Login successful
        }
    }
    return false; // Login failed
}
 
void User::registerUser(const std::string& filename) {
    std::string name, username, password, contact;
 
    std::cout << "\nEnter your full name: ";
    std::getline(std::cin, name);
 
    do {
        std::cout << "Choose a username: ";
        std::getline(std::cin, username);
 
        std::vector<User> users = readDataFromFile(filename);
 
        if (isUsernameTaken(username, users)) {
            std::cout << "Username is already taken. Please choose another.\n";
        }
 
    } while (isUsernameTaken(username, readDataFromFile(filename)));
 
    std::cout << "Choose a password: ";
    std::getline(std::cin, password);
 
    std::cout << "Enter your contact information: ";
    std::getline(std::cin, contact);
 
    std::vector<User> users = readDataFromFile(filename);
    users.emplace_back(name, username, password, contact);
    writeDataToFile(filename, users);
 
    std::cout << "\nUser registration successful!\n";
}

bool User::isAppointmentSlotAvailable(const std::string& providerUsername, const std::string& time, const std::string& appointmentsFilename){
    std::ifstream file(appointmentsFilename);
    std::string patientName, doctorUsername, appointmentTime;
    while(file >> patientName >> doctorUsername >> appointmentTime){
    if(providerUsername==doctorUsername && time==appointmentTime){
        file.close();
        return false;
    }
}

file.close();
return true;
}

void User::viewAppointments(const std::string& name, const std::string& appointmentsFilename){
    std::ifstream file(appointmentsFilename);
    int flag=0;
    std::string patientName, doctorUsername, appointmentTime;
    std::cout<<"\nPatient Name       Doctor username       Appointment Time\n\n";
    while(file>>patientName>>doctorUsername>>appointmentTime){
        if(patientName==name){
            flag=1;
            std::cout<<patientName<<"              "<<doctorUsername<<"              "<<appointmentTime<<"\n";
        } 
    }
    if(flag==0){
        std::cout<<"Sorry, you have no appointments scheduled\n";
    }
    file.close();
}

void User::scheduleAppointment(const std::string& providerUsername, const std::string& time, const std::string&inputUsername, const std::string& filename, const std::string& appointmentsFilename){
    if(isAppointmentSlotAvailable(providerUsername, time, appointmentsFilename)){
     std::vector<User> users = readDataFromFile(filename);

     auto it = std::find_if(users.begin(), users.end(), [inputUsername](const User& user){
        return user.username == inputUsername;
     });
     if(it!=users.end()){
        //it->contact += " " + providerUsername + " " + time;
        //writeDataToFile(filename, users);

        std::ofstream appointmentsFile("app.txt", std::ios::app);
        appointmentsFile<<it->name<<" "<<providerUsername<<" "<<time<<"\n";
        appointmentsFile.close();

        std::cout<<"Appointment scheduled with "<<providerUsername<<" for: "<< time<< "\n";

       //HealthcareProvider::scheduleAppointment(inputUsername, time, "pD.txt");
     } else{
        std::cout<<"User not found. Appointment scheduling failed.\n";
     }
    }
     else {
        std::cout<<"Appointment slot is already booked. Please choose another time or slot\n";
     }
 }

void User::sendMsg(const std::string& doctorUsername, const std::string& username) {
    std::string pipeName = "chat_" + username + "_" + doctorUsername;
 
    // Create a FIFO (named pipe) for communication
    mkfifo(pipeName.c_str(), 0666);
 
    std::ofstream chatOut(pipeName, std::ios::app);
 
    std::cout << "Chat started. Type 'exit' to end the chat.\n";
 
    std::string message;
    while (true) {
        std::getline(std::cin, message);
 
        if (message == "exit") {
            break;
        }
 
        // Send message to the doctor
        chatOut  << ": " << message << "\n";
        chatOut.flush();
    }
 
    // Close and remove the FIFO
    chatOut.close();
    unlink(pipeName.c_str());
}