#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

void print(int a){
    printf("%d\n", a);
}

void printc(char c){
    printf("%d\n", c);
}

void printchar(char c){
    printf("%d (%c)\n", c, c);
}

void alloc4(int **p, int a, int b, int c, int d){
    int *x = (int*)malloc(4*sizeof(int));
    x[0] = a;
    x[1] = b;
    x[2] = c;
    x[3] = d;
    *p = x;
}

void myassert(){
    assert(0);
}
