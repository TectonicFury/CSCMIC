#include <stdio.h>
#include <stdlib.h>
#include "../../includes/prime.h"
unsigned long long square(unsigned long long a) {
  return a * a;
}

unsigned long long remainder_(unsigned long long a, unsigned long long b) {
  return a % b;
}

unsigned long long expmod(unsigned long long base, unsigned long long exp, unsigned long long m) {
  if (exp == 0) return 1;
  if (exp % 2 == 0) {
    return remainder_(square(expmod(base, exp/2, m)), m);
  }
  return remainder_(base * expmod(base, exp - 1, m), m);
}

unsigned long long expmod_mod(unsigned long long base, unsigned long long exp, unsigned long long m) {
  if (remainder_(square(base), m) ==  1 && !(base == 1 || base == m - 1)) return 0;
  if (exp == 0) return 1;
  if (exp % 2 == 0) return remainder_(square(expmod_mod(base, exp/2, m)), m);
  return remainder_(base * expmod_mod(base, exp - 1, m), m);
}

int try_it(unsigned long long a, unsigned long long n) {
  return expmod(a, n, n) == a;
}

int fermat_test(unsigned long long n) {
  // nothing industrial
  return try_it(arc4random_uniform(n - 1) + 1, n);
}
int try_it_mod(unsigned long long a, unsigned long long n) {
  return expmod_mod(a, n - 1, n) == 1;
}
int fermat_test_mod(unsigned long long n) {
  // nothing industrial
  return try_it_mod(arc4random_uniform(n - 1) + 1, n);
}

int is_prime_miller_rabin(unsigned long long n, int times) {
  if (times == 0) return 1;
  if (fermat_test_mod(n)) return is_prime_miller_rabin(n, times - 1);
  return 0;
}

unsigned long long prime_twice(unsigned long long n) {
  unsigned long long x = 2 * n + 1;
  int times = 10;
  while (!is_prime_miller_rabin(x, times)) {
    x = x + 2;
  }
  return x;
}
int is_prime_fast(unsigned long long n, int times) {
  if (times == 0) return 1;
  if (fermat_test(n)) return is_prime_fast(n, times - 1);
  return 0;
}

unsigned long long next_prime(unsigned long long x) {
  int times = 10;
  if (x % 2 == 0) {
    x = x + 1;
    while (!is_prime_miller_rabin(x, times)) {
      x = x + 2;
    }
  } else {
    x = x + 2;
    while (!is_prime_miller_rabin(x, times)) {
      x = x + 2;
    }
  }
  return x;
}
// int main(int argc, char const *argv[]) {
//   // int times = 10;
//
//   // for (size_t i = 0; i < 1000; i++) {
//   //   if (is_prime_fast(i, times)) {
//   //     printf("result of miller_rabin = %d\n", is_prime_miller_rabin(i, times));
//   //     if (!is_prime_miller_rabin(i, times)) {
//   //       printf("Carmichael number = %zu\n", i);
//   //       continue;
//   //     }
//   //     printf("prime = %zu\n", i);
//   //   }
//   // }
//   printf("prime twice of 1111 is %llu\n", prime_twice(1111));
//   for (unsigned long long i = 2237, count = 0; count < 1000; count++) {
//     unsigned long long x = next_prime(i);
//     printf("next_prime = %llu\n", x);
//     i = x;
//   }
//   return 0;
// }
