#include <stdio.h>
#include <stdlib.h>

long long fib_iter(long long a, long long b, int i)
{
    if(i < 2) {
        return a;
    } else {
        return fib_iter(a + b, a, i - 1);
    }
}

long long fib(int n)
{
    return fib_iter(1, 0, n);
}

int main()
{
    fib(50000);    
    return 0;
}