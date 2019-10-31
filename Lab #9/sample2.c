#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SIZE 16

int num1;
int num2 = 5;

int func(int x){
    int temp[4] = {2,1,5, temp[0] * 2};
    return temp[3];
}

int main(){
    char *data1;
    char *data2 = malloc(10);

    data1 = malloc(SIZE);

    printf("Address of run time var pointer: %p\n", data1);
    printf("Address of run time var pointer: %p\n\n", data2);

    printf("Address of uninitialized num: %p\n", &num1);
    printf("Address of initilaized num: %p\n\n", &num2);

    printf("Address of library floor: %p\n", &floor);
    printf("Address of library ceil: %p\n\n", &ceil);

    printf("address of library asctime: %p\n", &asctime);
    printf("address of library strtok: %p\n\n", &strtok);

    int stack[3] = {1,2,3};
    int arr = func(7);
    int arr2 = func(34);
    printf("Address of stack func: %p\n", &arr);
    printf("Address of stack func: %p\n", &arr2);
    printf("Address of stack Var: %p\n\n", &stack);

    printf("Address of the program main: %p\n", &main);


    free(data1);
    free(data2);

    return 0;
}
