#ifndef __ASSAF_GMS_HPP__
#define __ASSAF_GMS_HPP__
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> //std::shared_ptr
#include<fstream>
#include "c_logger1.hpp"
#include "factory.hpp"
namespace assaf
{
    struct initParameters
    {
        std::string plateNum;
        std::string description;
        std::size_t yearOfManufacture;
        std::string status;
        std::size_t fixRate;
        std::size_t weightLimit;
        bool has_extraSeat;
    };
class Vehicle
{
    public:
    enum VehicleType
    {
        CAR = 0,
        MOTORCYCLE = 1,
        TRUCK = 2
    };
    explicit Vehicle(std::string plateNum,
                    std::string description,
                    std::size_t yearOfManufacture,
                    std::string status,
                    std::size_t fixRate);
    virtual ~Vehicle(){}
    virtual std::string GetPlateNum();
    virtual std::string GetDescription();
    virtual std::size_t GetYear();
    virtual std::string GetStatus();
    virtual void SetStatus(std::string);
    virtual std::size_t GetFixRate();

    private:
    std::string m_plateNum;
    std::string m_description;
    std::size_t m_yearOfManufacture;
    std::string m_status;
    std::size_t m_fixRate;
};

class Car: public Vehicle
{
    public: 
    Car(std::string plateNum,
        std::string description,
        std::size_t yearOfManufacture,
        std::string status);

};

class Motorcycle: public Vehicle
{
    public: 
    Motorcycle(std::string plateNum,
                std::string description,
                std::size_t yearOfManufacture,
                std::string status,
               bool has_extraSeat);
    
    private:
    bool m_hasExtraSeat;
};

class Truck: public Vehicle
{
    public:
    Truck(std::string plateNum,
        std::string description,
        std::size_t yearOfManufacture,
        std::string status,
        std::size_t weightLimit);

    
    private:
    std::size_t m_weightLimit;
    
};

class Garage
{
    public:
    Garage();
    void InsertCar(assaf::Vehicle::VehicleType type_ofVehicle, 
                   std::string plateNum,
                   std::string description,
                   std::size_t year,
                   std::string status,
                   std::size_t weightLimit,
                   bool hasExtraSeat);
    std::list<assaf::Vehicle *> GetListOfCarsByYears();
    void ChangeStatus(std::string plateNum, std::string newStatus);
    void ReleaseVehicle(std::string plateNum);
    void RecoverState();
    void ThreadFunction();
    assaf::Vehicle::VehicleType GetVehicleType(const std::string& vehicle);
    std::size_t GetProfit();
    
    private:
    std::string m_logFilePath;
    ilrd::CLogger *m_logger; // the logger is for error recovery
    std::unordered_map<std::string, std::shared_ptr<assaf::Vehicle *>> m_vehiclesMap;
    std::size_t m_totalProfit;
    ilrd::Factory<assaf::Vehicle *, std::string, assaf::initParameters> m_grageFactory;
    void EraseFileLine(std::string line_to_delete);
    void ReplaceStatus(std::string old_status, std::string new_status);

                             
};

}

#endif /* __ASSAF_GMS_HPP__ */