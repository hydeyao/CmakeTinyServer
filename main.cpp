#include <iostream>
#include "./log/LOG.h"

int get_factorial(int n)
{
    if (n == 1)
    {
        return 1;
    }
    return n * get_factorial(n - 1);
}

int main(int, char **)
{    
    return 0;
}
