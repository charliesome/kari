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
    printf("%lf\n", fib(100));
    return 0;
}