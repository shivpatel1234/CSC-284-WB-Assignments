#include "Fleet.h"
#include <iostream>

using namespace std;

Fleet::Fleet() {
}

Fleet::~Fleet() {
    // Delete all vehicles to prevent memory leaks
    for (size_t i = 0; i < vehicles.size(); ++i) {
        delete vehicles[i];
    }
    vehicles.clear();
    cout << "Fleet destroyed (all vehicles deleted)." << endl;
}

void Fleet::addVehicle(Vehicle* v) {
    if (v != nullptr) vehicles.push_back(v);
}

void Fleet::displayAllVehicles() const {
    cout << "--- Fleet Vehicles ---" << endl;
    for (size_t i = 0; i < vehicles.size(); ++i) {
        // virtual call chooses correct override
        vehicles[i]->displayInfo(); 
    }
}

double Fleet::getAverageEfficiency() const {
    if (vehicles.empty()) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < vehicles.size(); ++i) {
        sum += vehicles[i]->getFuelEfficiency();
    }
    return sum / vehicles.size();
}

int Fleet::size() const { return (int)vehicles.size(); }
