#include <stdio.h>

typedef struct Num {
  int numerator;
  int denominator;
} num;

int find_greatest_common_divisor(num n);
num shorten_fraction(num n);

int main() {
    num n = {1, 2};
    n = shorten_fraction(n);
    printf("%d/%d\n", n.numerator, n.denominator);
    return 0;
}

int find_greatest_common_divisor(num n) {
    int gcd = 1;
    while (n.numerator != n.denominator) {
        if (n.numerator > n.denominator) n.numerator -= n.denominator;
        else n.denominator -= n.numerator;
    }
    gcd = n.numerator;
    return gcd;
}

num shorten_fraction(num n) {
    int gcd = find_greatest_common_divisor(n); 
    n.numerator /= gcd;
    n.denominator /= gcd;
    return n;
}