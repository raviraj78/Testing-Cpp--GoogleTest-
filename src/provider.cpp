#include "provider.h"
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

std::string HealthcareProvider::getName() const {
    return Doctorname;
}

std::string HealthcareProvider::getUserName() const {
    return username;
}

std::string HealthcareProvider::getPassword() const {
    return password;
}

std::string HealthcareProvider::getSpecialty() const {
    return specialty;
}

std::string HealthcareProvider::getContact() const {
    return contact;
}

std::vector<HealthcareProvider> HealthcareProvider::readDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<HealthcareProvider> Providers;
    std::string Doctorname, username, password, specialty, contact;
 
    while (file >> Doctorname >> username >> password >> specialty >> contact) {
        Providers.emplace_back(Doctorname, username, password, specialty, contact);
    }
 
    file.close();
    return Providers;
}
 
void HealthcareProvider::writeDataToFile(const std::string& filename, const std::vector<HealthcareProvider>& Providers) {
    std::ofstream file(filename);
 
    for (const auto& provider : Providers) {
file << provider.Doctorname << " " << provider.username << " " << provider.password << " " << provider.specialty << " " << provider.contact << "\n";
    }
 
    file.close();
}

bool HealthcareProvider::isUsernameTaken(const std::string& username, const std::vector<HealthcareProvider>& Providers) {
    for (const auto& provider : Providers) {
        if (provider.username == username) {
            return true; // Username already taken
        }
    }
    return false; // Username is available
}

bool HealthcareProvider::loginUser(const std::string& username, const std::string& password, const std::vector<HealthcareProvider>& Providers) {
    for (const auto& provider : Providers) {
        if (provider.username == username && provider.password == password) {
            return true; // Login successful
        }
    }
    return false; // Login failed
}

void HealthcareProvider::registerUser(const std::string& filename) {
    std::string Doctorname, username, password, specialty, contact;
 
    std::cout << "\nEnter Doctor name: ";
    std::getline(std::cin, Doctorname);
 
    // Continue to prompt until a unique username is entered
    do {
        std::cout << "Choose a username: ";
        std::getline(std::cin, username);
 
        std::vector<HealthcareProvider> Providers = readDataFromFile(filename);
 
        if (isUsernameTaken(username, Providers)) {
            std::cout << "Username is already taken. Please choose another.\n";
        }
 
    } while (isUsernameTaken(username, readDataFromFile(filename)));
 
    std::cout << "Choose a password: ";
    std::getline(std::cin, password);
 
    std::cout << "Enter your specialization: ";
    std::getline(std::cin, specialty);

    std::cout << "Enter your contact information: ";
    std::getline(std::cin, contact);
 
    std::vector<HealthcareProvider> Providers = readDataFromFile(filename);
    Providers.emplace_back(Doctorname, username, password, specialty, contact);
    writeDataToFile(filename, Providers);
 
    std::cout << "\nDoctor registration successful!\n";
}

void HealthcareProvider::scheduleAppointment(const std::string& patientUsername, const std::string& time, const std::string& filename){
    std::vector<HealthcareProvider> providers = readDataFromFile(filename);

    auto it = std::find_if(providers.begin(), providers.end(), [&](const HealthcareProvider& provider){
        return provider.username==patientUsername;
    });
    if(it!=providers.end()){
        it->bookedAppointments.push_back(patientUsername+" "+time);
        writeDataToFile(filename, providers);
    }
}

void HealthcareProvider::viewAvailableProviders(const std::string& filename){
    std::vector<HealthcareProvider> providers = readDataFromFile(filename);

    std::cout<<"Available Healthcare Providers: \n";
    for(const auto& provider : providers){
        std::cout<<provider.Doctorname<<" - "<<provider.specialty<<"\n";
    }
}

bool HealthcareProvider::isProviderAvailable(const std::string& username, const std::string& time, const std::vector<std::string>& bookedAppointments){
    std::string providerSlot = username + " " +time;
    return std::find(bookedAppointments.begin(), bookedAppointments.end(), providerSlot)== bookedAppointments.end();
}

void HealthcareProvider::updateProviderSchedule(const std::string& username, const std::string& time, const std::string& patientUsername, const std::string& filename){
    std::vector<HealthcareProvider> providers = readDataFromFile(filename);
    for(auto& provider : providers){
        if(provider.username == username){
            provider.bookedAppointments.push_back(patientUsername +" " +time);
            writeDataToFile(filename, providers);
            return;
        }
    }
    std::cerr<<"Error: Provider not found for username "<<username<<std::endl;
}

void HealthcareProvider::viewPatients(const std::string& username, const std::string& appointmentsFilename){
    std::ifstream file(appointmentsFilename);
    int flag=0;
    std::string patientName, doctorUsername, appointmentTime;
    std::cout<<"\nPatient Name       Doctor username       Appointment Time\n\n";
    while(file>>patientName>>doctorUsername>>appointmentTime){
        if(doctorUsername==username){
            flag=1;
            std::cout<<patientName<<"              "<<doctorUsername<<"              "<<appointmentTime<<"\n";
        } 
    }
    if(flag==0){
        std::cout<<"Sorry, you have no appointments scheduled\n";
    }
    file.close();
}

void HealthcareProvider::sendMsg(const std::string& patientUsername, const std::string& username) {
    std::string pipeName = "chat_" + username + "_" + patientUsername;
 
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
 
        // Send message to the patient
        chatOut  << ": " << message << "\n";
        chatOut.flush();
    }
 
    // Close and remove the FIFO
    chatOut.close();
    unlink(pipeName.c_str());
}

void HealthcareProvider::cancelAppointment(const std::string& patientName, const std::string& appointmentTime, const std::string& appointmentsFilename){
    std::ifstream file(appointmentsFilename);
    std::vector<std::string> lines;
    std::string line;

    while(std::getline(file,line)) {
        lines.push_back(line);
    }
    file.close();

    std::ofstream outFile(appointmentsFilename, std::ios::trunc);
    outFile.close();

    std::ofstream outFileAppend(appointmentsFilename, std::ios::app);
    for(const auto& entry : lines) {
        std::istringstream iss(entry);
        std::string name, doctorUsername, time;
        iss>> name >> doctorUsername >>time;
        if(name==patientName && time==appointmentTime){
            std::cout<<"Appointment Cancelled for "<<patientName<<" at "<<appointmentTime<<"\n";
        }
        else{
            outFileAppend<<entry<<"\n";
        }
    }
}