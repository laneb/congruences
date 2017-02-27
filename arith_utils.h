#ifndef H_ARITH_UTILS
#define H_ARITH_UTILS
long mod_inv(long n, long mod);
long mod_product(long n1, long n2, long mod);
long mod_eval_polynomial(int degree, long coeffs[], long mod, long x);
#endif
