#ifndef H_PRIME_GEN
#define H_PRIME_GEN
long * primes(int length);

//Generation of primes is currently time-prohibitive when generating up to large maximums.
//This should be acceptable for prime factorization because although naive the algorithm is
//somewhat optimized to detect relatively large prime factors.
long * primes_upto(long max);
long * prime_factors(long n);
#endif
