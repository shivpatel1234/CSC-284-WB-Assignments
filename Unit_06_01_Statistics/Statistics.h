#pragma once
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <numeric>   // accumulate
#include <limits>    // numeric_limits (not strictly needed but handy)

using namespace std;

// ---------- Global Templated Functions ----------
// Constrain to arithmetic types for clearer errors at compile time.
template <typename T>
T getMin(const vector<T>& values) {
    static_assert(is_arithmetic<T>::value, "T must be a numeric type");
    if (values.empty()) throw invalid_argument("getMin: values is empty");
    T m = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i] < m) m = values[i];
    }
    return m;
}

template <typename T>
T getMax(const vector<T>& values) {
    static_assert(is_arithmetic<T>::value, "T must be a numeric type");
    if (values.empty()) throw invalid_argument("getMax: values is empty");
    T m = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        if (m < values[i]) m = values[i];
    }
    return m;
}

template <typename T>
double getAverage(const vector<T>& values) {
    static_assert(is_arithmetic<T>::value, "T must be a numeric type");
    if (values.empty()) throw invalid_argument("getAverage: values is empty");
    // sum as double to avoid integer division and overflow issues
    double sum = accumulate(values.begin(), values.end(), 0.0);
    return sum / static_cast<double>(values.size());
}

// ---------- Class Template Statistics<T> ----------
template <typename T>
class Statistics {
    static_assert(is_arithmetic<T>::value, "T must be a numeric type");

private:
    vector<T> data;

public:
    // Add one value
    void addValue(const T& value) {
        data.push_back(value);
    }

    // Optionally: bulk add (handy, not required)
    void addValues(const vector<T>& values) {
        data.insert(data.end(), values.begin(), values.end());
    }

    // Min/Max/Average mirror the global functions
    T getMin() const {
        if (data.empty()) throw invalid_argument("Statistics::getMin: no data");
        T m = data[0];
        for (size_t i = 1; i < data.size(); ++i) {
            if (data[i] < m) m = data[i];
        }
        return m;
    }

    T getMax() const {
        if (data.empty()) throw invalid_argument("Statistics::getMax: no data");
        T m = data[0];
        for (size_t i = 1; i < data.size(); ++i) {
            if (m < data[i]) m = data[i];
        }
        return m;
    }

    double getAverage() const {
        if (data.empty()) throw invalid_argument("Statistics::getAverage: no data");
        double sum = accumulate(data.begin(), data.end(), 0.0);
        return sum / static_cast<double>(data.size());
    }

    // Optional helpers (not required but useful in tests)
    const vector<T>& values() const { return data; }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};
