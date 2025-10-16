#pragma once
#include "Vehicle.h"

// Car derives from Vehicle
class Car : public Vehicle {
private:
    int numDoors;
    bool isElectric;

public:
    Car();
    Car(const string& mk, const string& mdl, int yr, double mi, int doors, bool electric);
    virtual ~Car();

    // Accessors
    int getNumDoors() const;
    bool getIsElectric() const;

    // Mutators
    void setNumDoors(int doors);
    void setIsElectric(bool electric);

    // Overrides
    virtual void displayInfo() const override;
    virtual double getFuelEfficiency() const override;
};
