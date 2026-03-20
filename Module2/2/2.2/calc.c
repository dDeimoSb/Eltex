#include "calc.h"
#include <math.h>

double add(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

double mul(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0){
        return NAN;
    }
    else{
        return a / b;
    }
}
