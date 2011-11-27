#include <stdio.h>
#include <stdlib.h>

double fib(int n)
{
    double a = 1, b = 0, c;
    int i;
    for(i = 1; i <= n; i++) {
        c = b;
        b = a;
        a = a + c;
    }
    return a;
}

int main()
{
    fib(100000);
    return 0;
}