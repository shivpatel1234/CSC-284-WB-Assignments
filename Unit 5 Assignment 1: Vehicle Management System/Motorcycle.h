#pragma once
#include "Vehicle.h"

// Motorcycle derives from Vehicle
class Motorcycle : public Vehicle {
private:
    bool hasSidecar;

public:
    Motorcycle();
    Motorcycle(const string& mk, const string& mdl, int yr, double mi, bool sidecar);
    virtual ~Motorcycle();

    // Accessors &Mutators
    bool getHasSidecar() const;
    void setHasSidecar(bool sidecar);

    // Overrides
    virtual void displayInfo() const override;
    virtual double getFuelEfficiency() const override;
};
