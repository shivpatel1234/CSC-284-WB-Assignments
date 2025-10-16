#pragma once
#include <string>
#include <iostream>

using namespace std;

// Base class for all vehicles
class Vehicle {
protected:
    string make;
    string model;
    int year;
    double mileage;

    // Validate helpers (kept simple)
    bool isValidYear(int y) const;      // true if 1886 <= y <= 2100
    bool isValidMileage(double m) const; // true if m >= 0

    // static counter of all Vehicle objects created
    static int vehicleCount;

public:
    // Constructors & destructor
    Vehicle();
    Vehicle(const string& mk, const string& mdl, int yr, double mi);
    virtual ~Vehicle(); // virtual so derived destructors run

    // Accessors
    string getMake() const;
    string getModel() const;
    int getYear() const;
    double getMileage() const;

    // Mutators
    void setMake(const string& mk);
    void setModel(const string& mdl);
    void setYear(int yr);
    void setMileage(double mi);

    // For static count
    static int getVehicleCount();

    // Polymorphic interface
    virtual void displayInfo() const;        // prints base info
    virtual double getFuelEfficiency() const = 0; // pure virtual
};

