#include <iostream>
#include <cstring>
#include "gms.hpp"

int main(void)
{
    assaf::Garage myGarage;
    std::string command;
    std::string vehicle;
    std::string plateNum = "";
    std::string description = "";
    std::size_t year = 0;
    std::size_t weightLimit = 0;
    bool has_extraSeat = false;
    
    
    myGarage.RecoverState();
    while (strncmp(command.c_str(), "exit", 5) != 0)
    {
        std::cout << "Enter your desired command and hit enter : insert/release/exit/change status" << std::endl;
        std::getline(std::cin >> std::ws,command);
 

        if (strncmp(command.c_str(), "insert", 6) == 0)
        {
            std::cout << "Enter the type of vehicle, plate number and year" << std::endl;
            std::cin >> vehicle >> plateNum >> year;
            std::cout << "Write the description of the problem " << std::endl;
            std::getline(std::cin >> std::ws,description);

            assaf::Vehicle::VehicleType vtype = myGarage.GetVehicleType(vehicle);
            switch (vtype)
            {
            case assaf::Vehicle::VehicleType::MOTORCYCLE:
                std::cout << "Does the motorcycle has extra seat? hit 1 for yes 0 for n" << std::endl;
                std::cin >> has_extraSeat;
                break;
            
            case assaf::Vehicle::VehicleType::TRUCK:
                std::cout << "enter truck weight limit: " << std::endl;
                std::cin >> weightLimit;
                break;
            }
            myGarage.InsertCar(vtype,
                              plateNum, description, year,"new",
                              weightLimit, has_extraSeat);
        }

        if (strncmp(command.c_str(), "release", 8) == 0)
        {
            std::cout << "enter the plate number of the vehicle" << std::endl;
            std::cin >> plateNum;
            myGarage.ReleaseVehicle(plateNum);   
        }
        if (strncmp(command.c_str(), "change status", 14) == 0)
        {
            std::cout << "enter the plate number of the vehicle: " << std::endl;
            std::cin >> plateNum ;
            std::cout << "enter the updated satatus: new/in fix/release";
            std::getline(std::cin >> std::ws,command);
            myGarage.ChangeStatus(plateNum, command);   
        }
    }
    myGarage.GetListOfCarsByYears();
    std::cout << "bye" << std::endl;
}