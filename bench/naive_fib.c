#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
    if(n < 2) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

int main()
{
    printf("fib(10) = %d\n", fib(10));
    printf("fib(20) = %d\n", fib(20));
    printf("fib(30) = %d\n", fib(30));
    
    return 0;
}