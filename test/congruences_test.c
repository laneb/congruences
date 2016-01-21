#include <stdio.h>
#include "../congruences.h"
#include "congruences_test.h"

void print_polynomial_inline(int func_degree, int * func_coeffs);
int solve_congruence_test(int func_degree, int * func_coeffs, int mod, int num_of_solutions, int * solutions);

int main(){
	int failures = 0;

	failures += solve_congruence_test(POL_1_DEGREE, POL_1_COEFFS, POL_1_MOD, NUM_OF_POL_1_SOLS, POL_1_SOLS);

	return failures;
}

int int_array_cmp_func(const void * a, const void * b){
	return *((int *)a) - *((int *) b);
}

int solve_congruence_test(int func_degree, int * func_coeffs, int mod, int num_of_solutions, int * solutions){
	int * solutions_to_test = solve_congruence(func_degree, func_coeffs, mod);
	int i, j;

	if(num_of_solutions != solutions_to_test[0]){
		printf("Incorrect number of solutions to congruence ");
		print_polynomial_inline(func_degree, func_coeffs);
		printf(" = 0: %d given instead of %d.\n", solutions_to_test[0], num_of_solutions);

		return 1;
	}

	qsort(solutions_to_test+1, solutions_to_test[0], sizeof(int), int_array_cmp_func);
	qsort(solutions, num_of_solutions, sizeof(int), int_array_cmp_func);

	for(i = 0; i < num_of_solutions; i++){
		if(solutions[i] != solutions_to_test[i+1]){
			printf("Incorrect %dth solution (after sorting) to congruence ", i);
			
			print_polynomial_inline(func_degree, func_coeffs);

			printf(" = 0: %d given instead of %d.\n", solutions_to_test[i+1], solutions[i]);

			return 1;
		}
	}

	printf("%d solutions found for congruence ", num_of_solutions);
	print_polynomial_inline(func_degree, func_coeffs);
	printf(" = 0 without error.\n");

	return 0;
}

void print_polynomial_inline(int func_degree, int * func_coeffs){
	int j;

	for(j = func_degree; j >= 1; j--){
		printf("%d*x^%d + ", func_coeffs[j], j);
	}

	printf("%d", func_coeffs[0]);
}