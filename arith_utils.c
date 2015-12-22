#include <stdio.h>
#include <stdlib.h>
#include "primes.c"



int mod_inv(int n, int mod);
int mod_product(int n1, int n2, int mod);
int mod_power(int n, int power, int mod);
int mod_eval_polynomial(int degree, int coeffs[], int mod, int x);
int coprime(int n1, int n2);

int * prime_factors(int n);
int totient(int n);
static int has_primitive_root(int n);
int least_primitive_root(int n);
int * primitive_roots(int n);


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
	int shareFactor = 1;
	int i;

	for(i=0; i<numOfFactors; i++){
		if(n2 % factors[i] == 0){
			shareFactor = 0;
			break;
		}
	}

	free(n1Factors);

	return shareFactor;
}


int mod_product(int num1, int num2, int mod){
	return (num1*num2) % mod;
}


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


int isPrimRoot(int g, int mod){
	int tot = totient(mod);
	int * totFactorList = prime_factors(tot);
	int numOfTotFactors = *totFactorList;
	int * totFactors = totFactorList+1;
	int i;

	for(i=0; i<numOfTotFactors; i++){
		if(mod_power(g, tot/totFactors[i], mod) == 1){
			return 0;
		}
	}

	free(totFactorList);

	return 1;
}


int has_primitive_root(int n){
	int * factorList = prime_factors(n);
	int numOfFactors = *factorList;
	int * factors = factorList+1;
	int has = 1;

	if(numOfFactors > 2 || n % 8 == 0 || (numOfFactors == 2 && factors[0] == 2 && n % 4 ==0)){
		has = 0;
	}

	free(factorList);

	return has;
}


int least_primitive_root(int n){
	int g;

	if(has_primitive_root(n)){
		for(g=2; g<n; g++){
			if(isPrimRoot(g, n)){
				return g;
			}
		}
	}

	return 0;
}


int * primitive_roots(int n){
	int g = least_primitive_root(n);
	int rootCount;
	int * rootList = NULL;
	int * roots;
	int tot;
	int power;

	if(g != 0){
		tot = totient(n);
		rootCount = totient(tot);
		rootList = calloc(rootCount + 1, sizeof(int));
		*rootList = rootCount;
		roots = rootList+1;
		*(roots++) = g;

		for(power=2; power < n; power++){
			if( coprime(tot, power) ){
				*(roots++) = mod_power(g, power, n);
			}
		}
	}

	return rootList;
}


int * prime_factors(int n){
	int divMax = (int) n;
	int * primeList = primes_upto( divMax );
	int numberOfPrimes = primeList[0];
	int * primes = primeList + 1;

	int * divisorList = (int *) calloc(numberOfPrimes+1, sizeof(int *));
	int * divisors = divisorList+1;
	int divListLength = 0;

	int i;

	for(i = 0; i < numberOfPrimes; i++){
		if(n % primes[i] == 0){
			divisors[divListLength++] = primes[i];
		}
	}


	if(divListLength == 0){
		divisors[0] = n;
		divListLength = 1;
	}

	divisorList[0] = divListLength;

	free(primeList);

	return divisorList;
}


int mod_eval_polynomial(int degree, int coeffs[], int mod, int x){
	int sum = coeffs[0];
	int i;

	for(i = 1; i <= degree; i++){
		sum += mod_power(x, i, mod)*coeffs[i];
		sum %= mod;
	}

	return sum;
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