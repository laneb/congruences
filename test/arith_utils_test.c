#include <stdio.h>
#include "../arith_utils.h"
#include "arith_utils_test.h"

int main(){
	int failures = 0;
	int i;

	for(i = 0; i < NUM_OF_MOD_INV_TESTS; i++){
		failures += mod_inv_test(MOD_INV_NUMS[i], MOD_INV_MODS[i], MOD_INV_INVS[i]);
	}

	for(i = 0; i < NUM_OF_MOD_PRODUCT_TESTS; i++){
		failures += mod_product_test(MOD_PRODUCT_NUM_PAIRS[i], MOD_PRODUCT_MODS[i], MOD_PRODUCT_PRODUCTS[i]);
	}

	for(i = 0; i < NUM_OF_MOD_EVAL_TESTS; i++){
		failures += mod_eval_polynomial_test(POLY_DEGREE[i], POLY_COEFFS[i], POLY_MOD[i], POLY_X[i], POLY_EVAL[i]);
	}

	printf("%d functions failing in arith_utils.c\n\n", failures);
	return failures;
}


int mod_product_test(int * pair, int mod, int product){
	int prod = mod_product(pair[0], pair[1], mod);

	if(prod != product){
		printf("Incorrect evaluation of %d*%d mod %d: %d instead of %d.\n",
				pair[0], pair[1], mod, prod, product);
		return 1;
	}

	return 0;
}


int mod_inv_test(int num, int mod, int inv){
	int invEval = mod_inv(num, mod);

	if(inv != invEval){
		printf("Incorrect evaluation of %d^-1 mod %d: %d instead of %d.\n", num, mod, invEval, inv);
		return 1;
	}

	return 0;
}

int mod_eval_polynomial_test(int degree, int coeffs[], int mod, int x, int val) {
	int eval = mod_eval_polynomial(degree, coeffs, mod, x);

	if(eval != val){
		printf("Incorrect evaliation of polynomial: %d instead of %d\n", eval, val);
		return 1;
	}

	return 0;
}
