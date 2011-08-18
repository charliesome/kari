#include <stdio.h>
#include <stdlib.h>

long long fib(int n)
{
    long long a = 1, b = 0, c, i;
    for(i = 0; i < n; i++) {
        c = b;
        b = a;
        a += c;
    }
    return a;
}

int main()
{
    fib(50000);    
    return 0;
}