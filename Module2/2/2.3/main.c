#include <stdio.h>
#include "calc.h"
#include <math.h>

int main() {
    Command commands[] = {
        {"Add", add},
        {"Substarction", sub},
        {"Multiplication", mul},
        {"Division", divide}
    };
    int command_count = sizeof(commands) / sizeof(commands[0]);
    int choice;
    double a, b, result;

    while (1){
        printf("\n--------------------\n");
        for (int i = 0; i < command_count; i++) {
            printf("%d. %s\n", i + 1, commands[i].name);
        }
        printf("0. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input error\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0) {
            printf("Exit\n");
            break;
        }

        if (choice < 1 || choice > command_count) {
            printf("Incorrect menu item\n");
            continue;
        }

        printf("Enter two numbers: ");

        if (scanf("%lf %lf", &a, &b) != 2) {
            printf("Error entering numbers\n");
            while (getchar() != '\n');
            continue;
        }

        
        result = commands[choice - 1].func(a, b);
        if (isnan(result) || isinf(result)) {
            printf("Error: cannot be divided by 0\n");
            continue;
        }
        
        printf("Result: %.2lf\n", result);
    }

    return 0;
}