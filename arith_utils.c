#include <stdio.h>
#include <stdlib.h>
#include "prime_gen.h"
#include "arith_utils.h"

//Expects 0 <= x,y < mod
int mod_sum(int x, int y, int mod){
	if(y >= mod - x){
		return y - (mod - x);
	}

	else{
		return y + x;
	}
}


int mod_inv(int n, int mod){
	int y, a;

	if(n!=0){

		while(n<0){
			n+=mod;
		}

		for(y = 1; y < mod; y++){
			a = mod_product(y, n, mod);

			if(a == 1){
				return y;
			}
		}
	}

	return 0;
}


int coprime(int n1, int n2){
	//naive algorithm but efficient when n1 has already been factorized
	int * n1Factors = prime_factors(n1);
	int numOfFactors = *n1Factors;
	int * factors = n1Factors+1;
	int shareFactor = 0;
	int i;

	for(i=0; i<numOfFactors; i++){
		if(n2 % factors[i] == 0){
			shareFactor = 1;
			break;
		}
	}

	free(n1Factors);

	return !shareFactor;
}


int mod_product(int num1, int num2, int mod){
	int prod = 0;
	int i;

	for(i = 0; i < num1; i++){
		prod = mod_sum(prod, num2, mod);
	}

	return prod;
}

//expects 0 <= n < mod
int mod_power(int n, int power, int mod){
	int product = n;
	int i;

	for(i = 1; i < power; i++){
		product = mod_product(product, n, mod);
	}

	return product;
}


int totient(int n){
	int * divisorList = prime_factors(n);
	int listLength = divisorList[0];
	int * divisors = divisorList+1;
	int i;

	for(i = 0; i < listLength; i++){
		n *= (divisors[i] - 1);
		n /= divisors[i];
	}

	free(divisorList);

	return n;
}




int mod_eval_polynomial(int degree, int coeffs[], int mod, int x){
	int tot = coeffs[degree];
	int i;

	for(i = degree - 1; i >= 0; i--){
		tot = mod_product(tot, x, mod);
		tot = mod_sum(tot, coeffs[i], mod);
	}

	return tot;
}


long  eval_polynomial(int degree, int coeffs[], int x){
	long int sum = coeffs[0];
 	long int powx;
	int i;

	for(i = 1, powx = x; i <= degree; i++, powx*=x){
		sum += powx*coeffs[i];
	}

	return sum;
}




/*
int * linear_diophantine_solution(int order, int coeffs[], int scal){

*=}
*/