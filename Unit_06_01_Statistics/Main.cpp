#include <iostream>
#include "Statistics.h"

using namespace std;

int main() {
    try {
        // ---- Test with int ----
        Statistics<int> intStats;
        intStats.addValue(10);
        intStats.addValue(20);
        intStats.addValue(30);

        cout << "Int Stats (class): Min = " << intStats.getMin()
            << ", Max = " << intStats.getMax()
            << ", Avg = " << intStats.getAverage() << "\n";

        // Also test the global function templates with the same data
        cout << "Int Stats (functions): Min = " << getMin(intStats.values())
            << ", Max = " << getMax(intStats.values())
            << ", Avg = " << getAverage(intStats.values()) << "\n\n";

        // ---- Test with double ----
        Statistics<double> doubleStats;
        doubleStats.addValue(3.14);
        doubleStats.addValue(2.71);
        doubleStats.addValue(4.5);

        cout << "Double Stats (class): Min = " << doubleStats.getMin()
            << ", Max = " << doubleStats.getMax()
            << ", Avg = " << doubleStats.getAverage() << "\n";

        cout << "Double Stats (functions): Min = " << getMin(doubleStats.values())
            << ", Max = " << getMax(doubleStats.values())
            << ", Avg = " << getAverage(doubleStats.values()) << "\n";

        // ---- (Optional) Another type: float ----
        Statistics<float> floatStats;
        floatStats.addValues({ 1.5f, 2.5f, 0.5f });
        cout << "\nFloat Stats (class): Min = " << floatStats.getMin()
            << ", Max = " << floatStats.getMax()
            << ", Avg = " << floatStats.getAverage() << "\n";

    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
