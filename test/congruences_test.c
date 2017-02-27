#include <stdio.h>
#include "../congruences.h"
#include "congruences_test.h"

void print_polynomial_inline(int func_degree, long * func_coeffs);
int solve_congruence_test(int func_degree, long * func_coeffs, long mod, int num_of_solutions, long * solutions);
void print_solutions_incorrect(int func_degree, long func_coeffs[], long mod, long exp_sol[], int num_exp_sol, long fnd_sol[], int num_found_sol);

int main(){
	int failures = 0;

	failures += solve_congruence_test(POL_1_DEGREE, POL_1_COEFFS, POL_1_MOD, NUM_OF_POL_1_SOLS, POL_1_SOLS);
	failures += solve_congruence_test(POL_2_DEGREE, POL_2_COEFFS, POL_2_MOD, NUM_OF_POL_2_SOLS, POL_2_SOLS);
	failures += solve_congruence_test(POL_3_DEGREE, POL_3_COEFFS, POL_3_MOD, NUM_OF_POL_3_SOLS, POL_3_SOLS);
	failures += solve_congruence_test(POL_4_DEGREE, POL_4_COEFFS, POL_4_MOD, NUM_OF_POL_4_SOLS, POL_4_SOLS);
	failures += solve_congruence_test(POL_5_DEGREE, POL_5_COEFFS, POL_5_MOD, NUM_OF_POL_5_SOLS, POL_5_SOLS);
	failures += solve_congruence_test(POL_6_DEGREE, POL_6_COEFFS, POL_6_MOD, NUM_OF_POL_6_SOLS, POL_6_SOLS);
	failures += solve_congruence_test(POL_7_DEGREE, POL_7_COEFFS, POL_7_MOD, NUM_OF_POL_7_SOLS, POL_7_SOLS);
	failures += solve_congruence_test(POL_8_DEGREE, POL_8_COEFFS, POL_8_MOD, NUM_OF_POL_8_SOLS, POL_8_SOLS);

	return failures;
}

int int_array_cmp_func(const void * a, const void * b){
	return *((long *)a) - *((long *) b);
}

int solve_congruence_test(int func_degree, long * func_coeffs, long mod, int num_of_solutions, long * solutions){
	long * solutions_to_test = solve_congruence(func_degree, func_coeffs, mod);
	int i, j;

	qsort(solutions_to_test+1, solutions_to_test[0], sizeof(long), int_array_cmp_func);
	qsort(solutions, num_of_solutions, sizeof(long), int_array_cmp_func);

	if(num_of_solutions != solutions_to_test[0]){
		print_solutions_incorrect(func_degree, func_coeffs, mod, solutions, num_of_solutions, solutions_to_test+1, solutions_to_test[0]);
		return 1;
	}

	for(i = 0; i < num_of_solutions; i++){
		if(solutions[i] != solutions_to_test[i+1]){
			print_solutions_incorrect(func_degree, func_coeffs, mod, solutions, num_of_solutions, solutions_to_test+1, solutions_to_test[0]);
			return 1;
		}
	}

	printf("Correct number of solutions (%d) found for congruence ", num_of_solutions);
	print_polynomial_inline(func_degree, func_coeffs);
	printf(" = 0 (mod %d) without error.\n\n", mod);

	return 0;
}

void print_solutions_incorrect(int func_degree, long func_coeffs[], long mod, long exp_sol[], int num_exp_sol, long fnd_sol[], int num_found_sol) {
		int i;

		printf("Incorrect solutions found for congruence ");
		print_polynomial_inline(func_degree, func_coeffs);
		printf(" = 0 (mod %d): %d given where %d expected.\n\n", mod, num_found_sol, num_exp_sol);

		printf("The following solutions were found: \n");
		for(i = 0; i < num_found_sol; i++){
			printf("(%d) %d\n", i, fnd_sol[i]);
		}

		printf("\nwhere the actual solutions are\n\n");

		for(i = 0; i < num_exp_sol; i++){
			printf("(%d) %d\n", i, exp_sol[i]);
		}
}

void print_polynomial_inline(int func_degree, long * func_coeffs){
	int j;

	for(j = func_degree; j >= 1; j--){
		printf("%d*x^%d + ", func_coeffs[j], j);
	}

	printf("%d", func_coeffs[0]);
}
