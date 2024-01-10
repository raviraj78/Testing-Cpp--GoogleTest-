#include<gtest/gtest.h>
#include"provider.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<filesystem>

TEST(ProviderTest, loginUser) {
    std::vector<HealthcareProvider> providers = {
        HealthcareProvider("Dr.Rohan", "DrRohan1","Rohan12121","Dentist", "8521469732"),
        HealthcareProvider("Dr.Tripti", "DrTripti","Tripti123","Surgical", "8547869732")
    };

    std::ofstream providerFile("testProviderData.txt");
    for(const auto& provider : providers){
        providerFile<<provider.getName()<<" "<<provider.getUserName()<<" "<<provider.getPassword()<<" "<<provider.getContact()<<"\n";
    }
    providerFile.close();

    EXPECT_TRUE(HealthcareProvider::loginUser("DrRohan1", "Rohan12121", HealthcareProvider::readDataFromFile("testProviderData.txt")));
    EXPECT_FALSE(HealthcareProvider::loginUser("invalid", "invalid121", HealthcareProvider::readDataFromFile("testProviderData.txt")));

    std::remove("testProviderdata.txt");
}

std::string getfile() {
    return "/tmp/user_test" +  std::to_string(std::rand()) + ".txt";
}

TEST(ProviderTest, registerUser) {

    std::string file = getfile();
    HealthcareProvider provider("Dr.Tripti", "DrTripti","Tripti123","Surgical", "8547869732");

    std::vector<HealthcareProvider> providers = {provider};

    HealthcareProvider::writeDataToFile(file, providers);
    HealthcareProvider::readDataFromFile(file);
    
    ASSERT_EQ("Dr.Tripti", provider.getName());
    ASSERT_EQ("DrTripti", provider.getUserName());

    std::remove(file.c_str());
} 