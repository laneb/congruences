#include "arith_utils.h"
#include <stdio.h>

#define NUM_OF_MOD_INV_TESTS 4
#define NUM_OF_MOD_PRODUCT_TESTS 4
#define NUM_OF_MOD_POWER_TESTS 4
#define NUM_OF_COPRIME_TESTS 5
#define NUM_OF_TOTIENT_TESTS 6
#define NUM_OF_MOD_EVAL_POLYNOMIAL_TESTS 1

int main(){
	int failures = 0;

	failures += mod_inv_test();
	failures += mod_product_test();
	failures += mod_power_test();
	failures += coprime_test();
	failures += totient_test();

	printf("%d functions failing in arith_utils.c\n", failures);
	return failures;
}


int totient_test(){
	int nums[NUM_OF_TOTIENT_TESTS] = {1, 2, 3, 41, 125, 9400};
	int totients[NUM_OF_TOTIENT_TESTS] = {1, 1, 2, 40, 100, 3680};
	int eval_totient;
	int i;

	for(i = 0; i < NUM_OF_TOTIENT_TESTS; i++){
		eval_totient = totient(nums[i]);
		if(totients[i] != eval_totient){
			printf("Totient of %d incorrectly evaluated: %d given instead of %d.\n", 
					nums[i], eval_totient, totients[i]);
			return 1;
		}
	}

	return 0;
}


int coprime_test(){
	int num_pairs[NUM_OF_COPRIME_TESTS][2] = {{3,5}, {9, 28}, {100, 34}, {1000512415, 557825}, {2286144, 1515839}};
	int is_coprime[NUM_OF_COPRIME_TESTS] = {1, 1, 0, 0, 1};
	int eval_coprime;
	int i;

	for(i = 0; i < NUM_OF_COPRIME_TESTS; i++){
		eval_coprime = coprime(num_pairs[i][0], num_pairs[i][1]);
		if(is_coprime[i] && !eval_coprime){
			printf("%d and %d incorrectly evaluated as not coprime.\n", 
					num_pairs[i][0], num_pairs[i][1]);
			return 1;
		}

		else if(!is_coprime[i] && eval_coprime){
			printf("%d and %d incorrectly evaluated as coprime.\n",
					num_pairs[i][0], num_pairs[i][1]);
			return 1;
		}
	}

	return 0;
}


int mod_power_test(){
	int nums[NUM_OF_MOD_POWER_TESTS] = {5, 19, 41, 100};
	int powers[NUM_OF_MOD_POWER_TESTS] = {10, 3, 4, 635};
	int mods[NUM_OF_MOD_POWER_TESTS] = {4, 23, 7, 33};
	int mdpwrs[NUM_OF_MOD_POWER_TESTS] = {1, 5, 1, 1};
	int mdpwr;
	int i;

	for(i = 0; i < NUM_OF_MOD_POWER_TESTS; i++){
		mdpwr = mod_power(nums[i], powers[i], mods[i]);
		if(mdpwr != mdpwrs[i]){
			printf("Incorrect evaluation of %d^%d mod %d: %d instead of %d.\n", 
					nums[i], powers[i], mods[i], mdpwr, mdpwrs[i]);
			return 1;
		}
	}

	return 0;
}


int mod_product_test(){
	int num_pairs[NUM_OF_MOD_PRODUCT_TESTS][2] = {{5,6}, {41,3}, {16, 98}, {105, 203}};
	int mods[NUM_OF_MOD_PRODUCT_TESTS] = {10, 8, 19, 98};
	int products[NUM_OF_MOD_PRODUCT_TESTS] = {0, 3, 10, 49};
	int prod;
	int i;

	for(i = 0; i < NUM_OF_MOD_PRODUCT_TESTS; i++){
		prod = mod_product(num_pairs[i][0], num_pairs[i][1], mods[i]);
		if(prod != products[i]){
			printf("Incorrect evaluation of %d*%d mod %d: %d instead of %d.\n", 
					num_pairs[i][0], num_pairs[i][1], mods[i], prod, products[i]);
			return 1;
		}
	}

	return 0;
}


int mod_inv_test(){
	int nums[NUM_OF_MOD_INV_TESTS] = {5, 4, 53, 129};
	int mods[NUM_OF_MOD_INV_TESTS] = {12, 23, 105, 7};
	int invs[NUM_OF_MOD_INV_TESTS] = {5, 6, 2, 5};
	int num_of_tests = NUM_OF_MOD_INV_TESTS;
	int inv;
	int i;

	for(i = 0; i < NUM_OF_MOD_INV_TESTS; i++){
		inv = mod_inv(nums[i], mods[i]);
		if(inv != invs[i]){
			printf("Incorrect evaluation of %d^-1 mod %d: %d instead of %d.\n", nums[i], mods[i], inv, invs[i]);
			return 1;
		}
	}

	return 0;
}

