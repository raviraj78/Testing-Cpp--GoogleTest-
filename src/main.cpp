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
#include"user.h"
#include"provider.h"
#include"client.h"
#include"server.h"

int main() {
    std::cout<<"-----------------------------------------------------------------------------------------\n";
    std::cout<<"                          MEDICAL APPOINTMENT SYSTEM                                     \n";
    std::cout<<"-----------------------------------------------------------------------------------------\n";

    std::cout << "Are you a patient or a healthcare provider?\n(Enter 'patient / user' or 'provider / doctor'): ";
    std::string userType;
    std::getline(std::cin, userType);
 
    std::string filename = (userType == "patient" || userType == "user" ) ? "uD.txt" : "pD.txt";
 
    std::cout << "\n1. Login\n2. Register\n\nChoose an option: ";
    int option;
    std::cin >> option;
    std::cin.ignore();
 
    switch (option) {
        case 1: {
            std::string username, password;
            std::cout << "\nEnter your username: ";
            std::getline(std::cin, username);
            std::cout << "Enter your password: ";
            std::getline(std::cin, password);

            if (userType == "patient" || userType == "user") {
                    bool loginSuccessful = User::loginUser(username, password, User::readDataFromFile(filename));
    
                    if (loginSuccessful) {
                        std::cout << "\n\nPatient login successful!";
                        std::cout<<"\n---------------------------WELCOME! "<<username<<"----------------------------\n";

                        std::cout<<"\n1. Schedule Appointment\n2. Send a msg to doctor\n3. View you appointments\n4. View Healthcare providers\n5. Chat with doctor\n\nChoose an option: ";
                        int patientOption;
                        std::cin >> patientOption;
                        std::cin.ignore();
                        switch(patientOption) {
                            case 1: {
                                std::cout<<"\nEnter the username of doctor you want appointment with: ";
                                std::string providerUsername;
                                std::getline(std::cin, providerUsername);

                                std::cout<<"Enter the time for appointment (format-10:00) : ";
                                std::string appointmentTime;
                                std::getline(std::cin, appointmentTime);

                                User::scheduleAppointment(providerUsername, appointmentTime, username, filename,"app.txt");
                                break;
                            }
                            case 2: {
                                std::cout<<"\nEnter the username of doctor you want to send msg: ";
                                std::string providerUsername;
                                std::getline(std::cin, providerUsername);
                                User::sendMsg(providerUsername,username);
                                break;
                            }
                            case 3: {
                                std::cout<<"\nEnter your name please, to view your appointments: ";
                                std::string name;
                                std::getline(std::cin,name);
                                User::viewAppointments(name,"app.txt");
                                break;
                            }
                            case 4: {
                                std::cout<<"\n";
                                HealthcareProvider::viewAvailableProviders("pD.txt");
                                break;
                            }
                            case 5: {
                                const int serverQueueKey = 1234;
                                Server server(serverQueueKey);
                                std::thread serverThread([&server](){
                                    server.startListening();
                                });

                                std::cout<<"\nEnter your username: ";
                                std::string username;
                                std::getline(std::cin,username);

                                Client userClient(username,serverQueueKey);
                                userClient.connectToServer();

                                std::cout<<"\nEnter the doctor's username you want to connect to: ";
                                std::string doctorUsername;
                                std::getline(std::cin,doctorUsername);

                                userClient.sendMessage("connect Doctor: "+doctorUsername);

                                std::cout<<"\nWaiting for doctor's response: ....";
                                std::string response = userClient.receiveMessage();
                                std::cout<<"\nReceived response: " <<response <<"\n";
                                serverThread.join();
                                break;
                            }
                            default:
                                std::cout<<"\nInvalid option. Exiting\n";
                                break;
                        }
                        
                    } else {
                        std::cout << "Patient login failed. Invalid username or password.\n";
                    }
                } else if (userType == "provider" || userType == "doctor") {
                    bool loginSuccessful = HealthcareProvider::loginUser(username, password, HealthcareProvider::readDataFromFile(filename));
    
                    if (loginSuccessful) {
                        std::cout << "\nHealthcare provider login successful!\n";
                        std::cout<<"------------------------WELCOME! "<<username<<"------------------------\n";

                        std::cout<<"\n1. View your patients\n2. Send a msg to patient\n3. Cancel Appointment\n4. Chat with patient\n\nChoose an option: ";
                        int providerOption;
                        std::cin >> providerOption;
                        std::cin.ignore();
                        switch(providerOption) {
                            case 1: {
                                std::cout<<"\n";
                                HealthcareProvider::viewPatients(username,"app.txt");
                                break;
                            }
                            case 2: {
                                std::string patientUsername;
                                std::cout<<"\nEnter patient's username to send msg: ";
                                std::getline(std::cin, patientUsername);

                                HealthcareProvider::sendMsg(patientUsername,username);
                                break;
                            }
                            case 3: {
                                std::string patientName;
                                std::cout<<"\nEnter patient's name to cancel appointment: ";
                                std::getline(std::cin, patientName);

                                std::string appointmentTime;
                                std::cout<<"Enter the time of appointment: ";
                                std::getline(std::cin, appointmentTime);

                                HealthcareProvider::cancelAppointment(patientName,appointmentTime,"app.txt");

                                break;
                            }
                            case 4: {
                                const int serverQueueKey = 1234;
                                Server server(serverQueueKey);
                                std::thread serverThread([&server](){
                                    server.startListening();
                                });

                                std::cout<<"\nEnter your username: ";
                                std::string username;
                                std::getline(std::cin,username);

                                Client doctorClient(username,serverQueueKey);
                                doctorClient.connectToServer();

                                std::cout<<"\nEnter the patient's username you want to connect to: ";
                                std::string patientUsername;
                                std::getline(std::cin,patientUsername);

                                doctorClient.sendMessage("connect Patient: "+ patientUsername);
                                
                                std::cout<<"\nWaiting for patient's response: ....";
                                std::string response = doctorClient.receiveMessage();
                                std::cout<<"\nReceived response: " <<response <<"\n";


                                serverThread.join();
                                break;
                            }
                            default:
                                std::cout<<"\nInvalid option. Exiting\n";
                                break;
                        }

                    } else {
                        std::cout << "Healthcare provider login failed. Invalid username or password.\n";
                    }
            } else {
                    std::cout << "Invalid user type. Exiting.\n";
                    }
            break;
        }
 
        case 2:
            if (userType == "patient" || userType == "user") {
                User::registerUser(filename);
            } else if (userType == "provider" || userType == "doctor") {
                HealthcareProvider::registerUser(filename);
            } else {
                std::cout << "Invalid user type. Exiting.\n";
            }
            break;
        default:
            std::cout << "Invalid option. Exiting.\n";
            break;
    }

    return 0;
}