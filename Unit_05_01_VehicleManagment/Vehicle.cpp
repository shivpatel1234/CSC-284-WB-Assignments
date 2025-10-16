#include "Vehicle.h"

// initialize static member
int Vehicle::vehicleCount = 0;

bool Vehicle::isValidYear(int y) const { return y >= 1886 && y <= 2100; }
bool Vehicle::isValidMileage(double m) const { return m >= 0.0; }

Vehicle::Vehicle()
    : make("Unknown"), model("Unknown"), year(2000), mileage(0.0) {
    vehicleCount++;
}

Vehicle::Vehicle(const string& mk, const string& mdl, int yr, double mi)
    : make("Unknown"), model("Unknown"), year(2000), mileage(0.0) {
    setMake(mk);
    setModel(mdl);
    setYear(yr);
    setMileage(mi);
    vehicleCount++;
}

Vehicle::~Vehicle() {
    vehicleCount--;
    cout << "Destroying Vehicle: " << make << " " << model << endl;
}

string Vehicle::getMake() const { return make; }
string Vehicle::getModel() const { return model; }
int Vehicle::getYear() const { return year; }
double Vehicle::getMileage() const { return mileage; }

void Vehicle::setMake(const string& mk) {
    
    if (!mk.empty()) make = mk;
}

void Vehicle::setModel(const string& mdl) {
    if (!mdl.empty()) model = mdl;
}

void Vehicle::setYear(int yr) {
    if (isValidYear(yr)) year = yr;
}

void Vehicle::setMileage(double mi) {
    if (isValidMileage(mi)) mileage = mi;
}

int Vehicle::getVehicleCount() { return vehicleCount; }

void Vehicle::displayInfo() const {
    cout << year << " " << make << " " << model << " | Mileage: " << mileage << " km" << endl;
}
