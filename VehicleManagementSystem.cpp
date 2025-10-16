#include <iostream>
#include <iomanip>
#include <limits>
#include "Fleet.h"
#include "Car.h"
#include "Truck.h"
#include "Motorcycle.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool askYesNo(const string& prompt) {
    char c;
    cout << prompt << " (y/n): ";
    cin >> c;
    clearInput();
    return (c == 'y' || c == 'Y');
}

void readBaseFields(string& make, string& model, int& year, double& mileage) {
    cout << "Enter make: ";
    getline(cin, make);
    if (make.empty()) make = "Unknown";

    cout << "Enter model: ";
    getline(cin, model);
    if (model.empty()) model = "Unknown";

    cout << "Enter year: ";
    while (!(cin >> year)) {
        clearInput();
        cout << "Enter a valid year: ";
    }
    clearInput();

    cout << "Enter mileage (km): ";
    while (!(cin >> mileage) || mileage < 0.0) {
        clearInput();
        cout << "Enter a valid non-negative mileage: ";
    }
    clearInput();
}

int main() {
    cout << "Vehicle Management System" << endl;
    cout << "--------------------------" << endl;

    Fleet fleet;
    int mainChoice = 0;

    while (true) {
        cout << "\nMain Menu\n";
        cout << "1) Add a vehicle\n";
        cout << "2) Display fleet\n";
        cout << "3) Exit\n";
        cout << "Choose option (1-3): ";
        if (!(cin >> mainChoice)) {
            clearInput();
            cout << "Invalid input.\n";
            continue;
        }
        clearInput();

        if (mainChoice == 3) {
            cout << "\nExiting program..." << endl;
            break;
        }

        if (mainChoice == 1) {
            cout << "\nChoose vehicle type:\n";
            cout << "1) Car\n";
            cout << "2) Truck\n";
            cout << "3) Motorcycle\n";
            cout << "Enter choice: ";

            int typeChoice;
            if (!(cin >> typeChoice)) {
                clearInput();
                cout << "Invalid choice.\n";
                continue;
            }
            clearInput();

            string make, model;
            int year;
            double mileage;
            cout << "\n-- Enter Vehicle Details --" << endl;
            readBaseFields(make, model, year, mileage);

            if (typeChoice == 1) {
                int doors;
                cout << "Enter number of doors (2-6): ";
                while (!(cin >> doors) || doors < 2 || doors > 6) {
                    clearInput();
                    cout << "Please enter between 2 and 6: ";
                }
                clearInput();
                bool electric = askYesNo("Is it electric?");
                Vehicle* car = new Car(make, model, year, mileage, doors, electric);
                fleet.addVehicle(car);
                cout << "Car added successfully.\n";
            }
            else if (typeChoice == 2) {
                double capacity;
                cout << "Enter load capacity (tons): ";
                while (!(cin >> capacity) || capacity < 0) {
                    clearInput();
                    cout << "Enter a non-negative number: ";
                }
                clearInput();
                Vehicle* truck = new Truck(make, model, year, mileage, capacity);
                fleet.addVehicle(truck);
                cout << "Truck added successfully.\n";
            }
            else if (typeChoice == 3) {
                bool sidecar = askYesNo("Does it have a sidecar?");
                Vehicle* m = new Motorcycle(make, model, year, mileage, sidecar);
                fleet.addVehicle(m);
                cout << "Motorcycle added successfully.\n";
            }
            else {
                cout << "Invalid vehicle type.\n";
            }
        }
        else if (mainChoice == 2) {
            cout << "\n--- Fleet Vehicles ---" << endl;
            if (fleet.size() == 0) {
                cout << "No vehicles in the fleet yet.\n";
            }
            else {
                fleet.displayAllVehicles();
                cout << "Total vehicles: " << Vehicle::getVehicleCount() << endl;
                cout << fixed << setprecision(2);
                cout << "Average fuel efficiency: "
                    << fleet.getAverageEfficiency() << " km/l" << endl;
            }
        }
        else {
            cout << "Invalid option.\n";
        }
    }

    cout << "\nProgram ending... destructors will now be called automatically.\n";
    return 0;
}
