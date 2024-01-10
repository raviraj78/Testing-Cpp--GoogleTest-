#ifndef PROVIDER_H
#define PROVIDER_H

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

class HealthcareProvider {
public:
    std::string Doctorname;
    std::string username;
    std::string password;
    std::string specialty;
    std::string contact;
    std::vector<std::string> bookedAppointments;
 
    HealthcareProvider(const std::string& n, const std::string& u, const std::string& p,const std::string& s, const std::string& c)
        : Doctorname(n), username(u), password(p), specialty(s), contact(c) {}
    
    std::string getName() const;
    std::string getUserName() const;
    std::string getPassword() const;
    std::string getSpecialty() const;
    std::string getContact() const;
 
    static std::vector<HealthcareProvider> readDataFromFile(const std::string& filename);
    static void writeDataToFile(const std::string& filename, const std::vector<HealthcareProvider>& Providers);

    static bool isUsernameTaken(const std::string& username, const std::vector<HealthcareProvider>& Providers);
    static bool loginUser(const std::string& username, const std::string& password, const std::vector<HealthcareProvider>& Providers);
    static void registerUser(const std::string& filename);
    static void scheduleAppointment(const std::string& patientUsername, const std::string& time, const std::string& filename);
    static void viewAvailableProviders(const std::string& filename);
    static bool isProviderAvailable(const std::string& username, const std::string& time, const std::vector<std::string>& bookedAppointments);
    static void updateProviderSchedule(const std::string& username, const std::string& time, const std::string& patientUsername, const std::string& filename);
    static void viewPatients(const std::string& username, const std::string& appointmentsFilename);
    static void sendMsg(const std::string& patientUsername,const std::string& username);
    static void cancelAppointment(const std::string& patientName, const std::string& appointmentTime, const std::string& appointmentsFilename);
};
#endif