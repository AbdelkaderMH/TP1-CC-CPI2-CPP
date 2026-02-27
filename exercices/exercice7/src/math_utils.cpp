#include "math_utils.hpp"

double moyenne(int a, int b) {
    return (a + b) / 2.0;
}

bool division_securisee(int a, int b, double& r) {
    if (b == 0) {
        return false;  // Division by zero
    }
    r = static_cast<double>(a) / b;
    return true;
}