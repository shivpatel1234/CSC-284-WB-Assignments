#pragma once
#include <vector>
#include "Vehicle.h"

using namespace std;

class Fleet {
private:
    vector<Vehicle*> vehicles;

public:
    Fleet();
    ~Fleet(); // deletes all Vehicle* to avoid memory leaks

    void addVehicle(Vehicle* v);          // takes ownership
    void displayAllVehicles() const;      // polymorphic calls
    double getAverageEfficiency() const;  // average of getFuelEfficiency()
    int size() const;                     // how many vehicles stored
};

