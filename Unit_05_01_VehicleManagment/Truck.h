#pragma once
#include "Vehicle.h"

// Truck derives from Vehicle
class Truck : public Vehicle {
private:
    double loadCapacity; //tons

public:
    Truck();
    Truck(const string& mk, const string& mdl, int yr, double mi, double capacity);
    virtual ~Truck();

    // Accessors & mutators
    double getLoadCapacity() const;
    void setLoadCapacity(double capacity);

    // Overrides
    virtual void displayInfo() const override;
    virtual double getFuelEfficiency() const override;
};

