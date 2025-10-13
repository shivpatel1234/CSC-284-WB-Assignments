#include "Car.h"

Car::Car()
    : Vehicle(), numDoors(4), isElectric(false) {
}

Car::Car(const string& mk, const string& mdl, int yr, double mi, int doors, bool electric)
    : Vehicle(mk, mdl, yr, mi), numDoors(4), isElectric(false) {
    setNumDoors(doors);
    setIsElectric(electric);
}

Car::~Car() {
    cout << "Destroying Car: " << make << " " << model << endl;
}

int Car::getNumDoors() const { return numDoors; }
bool Car::getIsElectric() const { return isElectric; }

void Car::setNumDoors(int doors) {
    if (doors >= 2 && doors <= 6) numDoors = doors;
}

void Car::setIsElectric(bool electric) { isElectric = electric; }

double Car::getFuelEfficiency() const {

    if (isElectric) return 0.0;
    double base = 15.0;

    if (numDoors > 4) base -= 1.0;
    return base;
}

void Car::displayInfo() const {
    Vehicle::displayInfo();
    
    cout << "Type: Car | Doors: " << numDoors
        << " | Electric: " << (isElectric ? "Yes" : "No") << endl;
    cout << "Fuel Efficiency: " << getFuelEfficiency() << " km/l" << endl << endl;
}
