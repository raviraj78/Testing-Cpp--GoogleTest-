#include<gtest/gtest.h>
#include"user.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<filesystem>

TEST(UserTest, loginUser) {
    std::vector<User> users = {
        User("Rohan", "Rohan121","Rohan123", "8521469732"),
        User("Tripti", "Tripti121","Tripti123", "8547869732")
    };

    std::ofstream userFile("testUserData.txt");
    for(const auto& user : users){
        userFile<<user.getName()<<" "<<user.getUserName()<<" "<<user.getPassword()<<" "<<user.getContact()<<"\n";
    }
    userFile.close();

    EXPECT_TRUE(User::loginUser("Rohan121", "Rohan123", User::readDataFromFile("testUserData.txt")));
    EXPECT_FALSE(User::loginUser("invalid", "invalid121", User::readDataFromFile("testUserData.txt")));

    std::remove("testUserdata.txt");
}

std::string getfile() {
    return "/tmp/user_test" +  std::to_string(std::rand()) + ".txt";
}

TEST(UserTest, registerUser) {

    std::string file = getfile();
    User user("Tripti", "Tripti121","Tripti123", "8547869732");

    std::vector<User> users = {user};

    User::writeDataToFile(file, users);
    User::readDataFromFile(file);
    
    ASSERT_EQ("Tripti", user.getName());
    ASSERT_EQ("Tripti121", user.getUserName());

    std::remove(file.c_str());
} 

// TEST(UserTest, ScheduleAppointment) {
//     std::cout<<"Current directory: "<< std::filesystem::current_path() <<std::endl;

//     std::string patientUsername = "john_patient";
//     std::string providerUsername = "dr_smith";
//     std::string appointmentTime = "10:00AM";
//     std::string filename = "../data/uD.txt";  
//     std::string appointmentFileName = getfile();

//     std::ofstream userFile(filename);
//     userFile << "JohnDoe " << patientUsername << " some_password 123456789";
//     userFile.close();

//     User patient("JohnDoe", patientUsername, "some_password", "123456789");

//     EXPECT_TRUE(std::filesystem::exists(filename)) <<"Data file does not exist";

//     testing::internal::CaptureStdout();
//     patient.scheduleAppointment(providerUsername, appointmentTime, patientUsername, filename, appointmentFileName);
//     std::string capturedOutput = testing::internal::GetCapturedStdout();  

//     std::stringstream expectedOutput;
//     expectedOutput << "Appointment scheduled with " << providerUsername << " for: " << appointmentTime << "\n";
//     EXPECT_EQ(expectedOutput.str(), capturedOutput);


//     std::ifstream appointmentFileIn(appointmentFileName);


//     EXPECT_TRUE(appointmentFileIn.is_open()) << "Failed to open the appointment file.";

//     std::stringstream buffer;
//     buffer << appointmentFileIn.rdbuf();
//     std::string actualAppointmentContent = buffer.str();

//     std::string expectedAppointmentDetails = "JohnDoe dr_smith 10:00AM";


//     EXPECT_EQ(expectedAppointmentDetails, actualAppointmentContent);

//     // Close the file
//     appointmentFileIn.close();
// }