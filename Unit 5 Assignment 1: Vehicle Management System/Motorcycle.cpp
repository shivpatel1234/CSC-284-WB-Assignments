#include "Motorcycle.h"

Motorcycle::Motorcycle()
    : Vehicle(), hasSidecar(false) {
}

Motorcycle::Motorcycle(const string& mk, const string& mdl, int yr, double mi, bool sidecar)
    : Vehicle(mk, mdl, yr, mi), hasSidecar(false) {
    setHasSidecar(sidecar);
}

Motorcycle::~Motorcycle() {
    cout << "Destroying Motorcycle: " << make << " " << model << endl;
}

bool Motorcycle::getHasSidecar() const { return hasSidecar; }
void Motorcycle::setHasSidecar(bool sidecar) { hasSidecar = sidecar; }

double Motorcycle::getFuelEfficiency() const {
    // rule: 32 km/l without sidecar, 28 with sidecar 
    return hasSidecar ? 28.0 : 32.0;
}

void Motorcycle::displayInfo() const {
    Vehicle::displayInfo();
    cout << "Type: Motorcycle | Sidecar: " << (hasSidecar ? "Yes" : "No") << endl;
    cout << "Fuel Efficiency: " << getFuelEfficiency() << " km/l" << endl << endl;
}
