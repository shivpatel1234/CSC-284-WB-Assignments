#include "Truck.h"

Truck::Truck()
    : Vehicle(), loadCapacity(1.0) {
}

Truck::Truck(const string& mk, const string& mdl, int yr, double mi, double capacity)
    : Vehicle(mk, mdl, yr, mi), loadCapacity(1.0) {
    setLoadCapacity(capacity);
}

Truck::~Truck() {
    cout << "Destroying Truck: " << make << " " << model << endl;
}

double Truck::getLoadCapacity() const { return loadCapacity; }

void Truck::setLoadCapacity(double capacity) {
    if (capacity >= 0.0) loadCapacity = capacity;
}

double Truck::getFuelEfficiency() const {
    // rule: base 9 km/l, minus 0.1 per ton
    double base = 9.0 - (0.1 * loadCapacity);
    
    if (base < 3.0) base = 3.0;
    return base;
}

void Truck::displayInfo() const {
    Vehicle::displayInfo();
    cout << "Type: Truck | Load Capacity: " << loadCapacity << " tons" << endl;
    cout << "Fuel Efficiency: " << getFuelEfficiency() << " km/l" << endl << endl;
}
