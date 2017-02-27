#ifndef H_ARITH_UTILS
#define H_ARITH_UTILS
int mod_inv(int n, int mod);
int mod_product(int n1, int n2, int mod);
int mod_eval_polynomial(int degree, int coeffs[], int mod, int x);
#endif
