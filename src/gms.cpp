#include <iostream>
#include <algorithm>
#include <thread>
#include <cstring>
#include <iterator>
#include <sstream>
#include "gms.hpp"
#include "c_logger1.hpp"

namespace assaf
{
    assaf::Vehicle::VehicleType Garage::GetVehicleType(const std::string& vehicle)
    {
        if (strncmp(vehicle.c_str(), "car" ,4)== 0)
        {
            return assaf::Vehicle::VehicleType::CAR;
        }

        if (strncmp(vehicle.c_str(), "motorcycle" ,11)== 0)
        {
            return assaf::Vehicle::VehicleType::MOTORCYCLE;
        }

        if (strncmp(vehicle.c_str(), "truck" ,6)== 0)
        {
            return assaf::Vehicle::VehicleType::TRUCK;
        }
    }
    Vehicle::Vehicle(std::string plateNum,
                    std::string description,
                    std::size_t yearOfManufacture,
                    std::string status,
                    std::size_t fixRate): m_plateNum(plateNum),
                                          m_description(description),
                                          m_yearOfManufacture(yearOfManufacture),
                                          m_status(status),
                                          m_fixRate(fixRate)
    {
        //empty ctor
    }
    std::string Vehicle::GetPlateNum()
    {
        return m_plateNum;
    }

    std::string Vehicle::GetDescription()
    {
        return m_description;
    }

    std::size_t Vehicle::GetYear()
    {
        return m_yearOfManufacture;
    }

    std::string Vehicle::GetStatus()
    {
        return m_status;
    }

    std::size_t Vehicle::GetFixRate()
    {
        return m_fixRate;
    }
    void Vehicle::SetStatus(std::string stat)
    {
        m_status = stat;
    }
    Car::Car(std::string plateNum,
        std::string description,
        std::size_t yearOfManufacture,
        std::string status): Vehicle(plateNum,
                                    description,
                                    yearOfManufacture,
                                    status,
                                    2000)
    {
        //
    }
    
    Motorcycle::Motorcycle(std::string plateNum,
                std::string description,
                std::size_t yearOfManufacture,
                std::string status,
               bool has_extraSeat):  Vehicle(plateNum,
                                               description,
                                               yearOfManufacture,
                                               status,
                                               1000), 
                                    m_hasExtraSeat(has_extraSeat)
    {
        //
    }


    Truck::Truck(std::string plateNum,
                std::string description,
                std::size_t yearOfManufacture,
                std::string status,
                std::size_t weightLimit):Vehicle(plateNum,
                                               description,
                                               yearOfManufacture,
                                               status,
                                               5000),
                                               m_weightLimit(weightLimit)
    {
        //
    }
    
    std::shared_ptr<assaf::Vehicle *> CreateCar(assaf::initParameters params)
    {
        return std::make_shared<assaf::Vehicle *>(new assaf::Car(params.plateNum, 
                                                  params.description,
                                                  params.yearOfManufacture,
                                                  params.status));
    }
    std::shared_ptr<assaf::Vehicle *> CreateMotor(assaf::initParameters params)
    {
        return std::make_shared<assaf::Vehicle *>(new assaf::Motorcycle(params.plateNum, 
                                                 params.description,
                                                 params.yearOfManufacture,
                                                 params.status,
                                                 params.has_extraSeat));
    }
    std::shared_ptr<assaf::Vehicle *> CreateTruck(assaf::initParameters params)
    {
        return std::make_shared<assaf::Vehicle *>(new assaf::Truck(params.plateNum,
                                                    params.description,
                                                 params.yearOfManufacture,
                                                 params.status,
                                                 params.weightLimit));
    }
    Garage::Garage():m_logFilePath("/home/assaf/git/projects/gms/garage1_log.txt")
    {
        m_grageFactory.Add("car", CreateCar);
        m_grageFactory.Add("motorcycle", CreateMotor);
        m_grageFactory.Add("truck", CreateTruck);
    }

    
    void Garage::InsertCar(assaf::Vehicle::VehicleType type_ofVehicle, 
                   std::string plateNum,
                   std::string description,
                   std::size_t year,
                   std::string status,
                   std::size_t weightLimit,
                   bool hasExtraSeat)
    {
        //use factory of vehicle
        std::shared_ptr<assaf::Vehicle *>newVehicle;
        struct assaf::initParameters params = {
            .plateNum = plateNum,
            .description = description,
            .yearOfManufacture = year,
            .status = status,
            .weightLimit = weightLimit,
            .has_extraSeat = hasExtraSeat
        };
        switch (type_ofVehicle)
        {
        case assaf::Vehicle::VehicleType::CAR:
            newVehicle = m_grageFactory.Create("car", params);

            m_logger->GetInstance(m_logFilePath.c_str())->Log("%s %s %ld %s %s", 
                                        "car",
                                        plateNum.c_str(),
                                        year,
                                        status.c_str(),
                                        description.c_str());
            break;
        
        case assaf::Vehicle::VehicleType::MOTORCYCLE:
            newVehicle = m_grageFactory.Create("motorcycle", params);
            m_logger->GetInstance(m_logFilePath.c_str())->Log("%s %s %s %ld %s %d", 
                                        "motorcycle",
                                        plateNum.c_str(),
                                        description.c_str(),
                                        year,
                                        status.c_str(),
                                        false);
            break;
        
        case assaf::Vehicle::VehicleType::TRUCK:
            newVehicle = m_grageFactory.Create("truck", params);
            m_logger->GetInstance(m_logFilePath.c_str())->Log("%s %s %s %ld %s %ld", 
                                        "truck",
                                        plateNum.c_str(),
                                        description.c_str(),
                                        year,
                                        status.c_str(),
                                        weightLimit);
            break;
        
        default:
            break;
        }
        
        m_vehiclesMap[plateNum] = newVehicle;

    }

    void Garage::EraseFileLine(std::string line_to_delete)
    {
        std::string line;
        std::ifstream fin;
        
        fin.open(m_logFilePath.c_str());
        // contents of path must be copied to a temp file then
        // renamed back to the path file
        std::ofstream temp;
        temp.open("temp.txt");

        while (getline(fin, line)) {
            // write all lines to temp other than the line marked for erasing
            if (line.find(line_to_delete)== std::string::npos )
            {
                temp << line << std::endl;
            }
        }

        temp.close();
        fin.close();

        // required conversion for remove and rename functions
        const char * p = m_logFilePath.c_str();
        remove(p);
        rename("temp.txt", p);
    }
    void Garage::ThreadFunction()
    {
        std::shared_ptr<assaf::Vehicle *>newVehicle;
        std::ifstream logFile(m_logFilePath.c_str());
        std::string mock[4];
        std::string vehicle;
        struct assaf::initParameters params;
        std::string line;

        while(getline(logFile, line))
        {
            std::istringstream ss(line);

            ss >> mock[0] >> mock[1] >> mock[2] >> mock[3] >> vehicle >> params.plateNum >> params.yearOfManufacture >>
            params.status >> params.description >> params.weightLimit >> params.has_extraSeat;
            std::cout << vehicle  << params.plateNum<<std::endl;
            assaf::Vehicle::VehicleType vtype = GetVehicleType(vehicle);
            switch (vtype)
            {
            case assaf::Vehicle::VehicleType::MOTORCYCLE:
                ss >> params.has_extraSeat;
                break;
            
            case assaf::Vehicle::VehicleType::TRUCK:
                ss >> params.weightLimit;
                break;
            }
            newVehicle = m_grageFactory.Create(vehicle, params);
            m_vehiclesMap[params.plateNum] = newVehicle;
            std::cout << m_vehiclesMap.size() << std::endl;
        }
        
    }
    void Garage::RecoverState()
    {
        std::ifstream in_file(m_logFilePath.c_str());
        in_file.seekg(0, std::ios::end);
        int file_size = in_file.tellg();
        if (file_size > 0)
        {
            std::cout << "recovering" << std::endl;
            std::thread(&assaf::Garage::ThreadFunction, std::ref(*this)).detach();
        }
    }
    void Garage::ReleaseVehicle(std::string plateNum)
    {
        m_vehiclesMap.erase(plateNum);
        std::thread(&assaf::Garage::EraseFileLine, std::ref(*this), plateNum).detach();

        std::cerr << "Vehicle " << plateNum << " is released" << std::endl;
    }
    void Garage::ReplaceStatus(std::string old_status, std::string new_status)
    {
        std::fstream file(m_logFilePath, std::ios::in |
                          std::ios::out | std::ios::ate);
        if (file.is_open()) 
        {
            std::string line;
            std::streampos currentPos = file.tellg();
            while (getline(file, line)) {
            if (line.find(old_status) != std::string::npos) 
            {
                file.seekp(currentPos);
                file << new_status ;
                break;
            }
            currentPos = file.tellg();
            }
            file.close();
        } else {
            std::cout << "Unable to open file";
        }
    }

    
    void Garage::ChangeStatus(std::string plateNum, std::string newStatus)
    {
        (*m_vehiclesMap.at(plateNum))->SetStatus(newStatus);
        std::thread(&assaf::Garage::ReplaceStatus, std::ref(*this), plateNum, newStatus).detach();
        
    }
    bool cmpYears (assaf::Vehicle * first, assaf::Vehicle * second)
    {
        if (first->GetYear() < second->GetYear())
        {
            return true;
        }
        return false;
     
    }
    
    std::list<assaf::Vehicle *> Garage::GetListOfCarsByYears()
    {
        std::list<assaf::Vehicle *> result; 
        for (auto iter = m_vehiclesMap.begin(); iter != m_vehiclesMap.end(); ++iter)
        {
            result.push_back(*iter->second);
        }

        result.sort(cmpYears);

        for (auto iter = result.begin() ; iter != result.end(); ++iter)
        {
            std::cout << (*iter)->GetYear() << std::endl;// contents of path must be copied to a temp file then
        // renamed back to the path file
        }
        return result;
    }

}
