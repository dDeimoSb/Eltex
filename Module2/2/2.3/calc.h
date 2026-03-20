#ifndef CALC_H
#define CALC_H

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double divide(double a, double b);

typedef double (*OperationFunc)(double, double);

typedef struct {
    char name[20];
    OperationFunc func;
} Command;

#endif
