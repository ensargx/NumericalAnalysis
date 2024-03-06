#include <stdio.h>

long long power(long long base, int exponent)
{
    if (exponent == 1) {
        return base;
    }
    long long result = 1;
    if (exponent % 2 == 0) {
        result = power(base, exponent / 2);
        return result * result;
    }
    else {
        result = power(base, (exponent - 1) / 2);
        return base * result * result;
    }
}

int main()
{
    long long base = 2;
    int exponent = 3;
    long long result = power(base, exponent);
    printf("The result of %lld raised to the power of %d is %lld\n", base, exponent, result);
    return 0;
}
