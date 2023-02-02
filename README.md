Garage API

Overview

The Garage class provides a set of functions to manage a garage that stores vehicles of different types. The garage keeps track of each vehicle's information including type, plate number, description, year, status, weight limit, and whether it has an extra seat or not. The Garage class also provides functions to update the status of a vehicle, release a vehicle, and recover the state of the garage after a crash or an error.

Class Methods

Garage()

This is the constructor of the Garage class. It initializes the necessary data members of the class.

InsertCar(VehicleType type_ofVehicle, string plateNum, string description, size_t year, string status, size_t weightLimit, bool hasExtraSeat)

This method inserts a new vehicle into the garage. The input parameters include the type of vehicle, plate number, description, year, status, weight limit, and whether it has an extra seat or not.

GetListOfCarsByYears()

This method returns a list of vehicles stored in the garage sorted by the year of the vehicles.

ChangeStatus(string plateNum, string newStatus)

This method changes the status of a vehicle with a given plate number to a new status.

ReleaseVehicle(string plateNum)

This method releases a vehicle with a given plate number from the garage.

RecoverState()

This method is used to recover the state of the garage after a crash or an error.

ThreadFunction()

This method is used to run the garage in a separate thread.

GetVehicleType(const string& vehicle)

This method returns the type of a vehicle with a given name.

GetProfit()

This method returns the total profit earned by the garage.
