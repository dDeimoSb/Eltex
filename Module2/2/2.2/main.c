#include <stdio.h>
#include "calc.h"

int main() {
    int choice;
    double a, b, result;

    while (1) {
        printf("\n--------------------\n");
        printf("1. Add\n");
        printf("2. Substarction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
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

        if (choice < 1 || choice > 4) {
            printf("Incorrect menu item\n");
            continue;
        }

        printf("Enter two numbers: ");
        if (scanf("%lf %lf", &a, &b) != 2) {
            printf("Error entering numbers\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                result = add(a, b);
                printf("Result: %.2lf\n", result);
                break;
            case 2:
                result = sub(a, b);
                printf("Result: %.2lf\n", result);
                break;
            case 3:
                result = mul(a, b);
                printf("Result: %.2lf\n", result);
                break;
            case 4:
                if (b == 0) {
                    printf("Error: cannot be divided by 0\n");
                } else {
                    result = divide(a, b);
                    printf("Result: %.2lf\n", result);
                }
                break;
        }
    }

    return 0;
}
