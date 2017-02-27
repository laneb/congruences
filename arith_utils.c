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


int mod_product(int num1, int num2, int mod){
	int prod = 0;
	int i;

	for(i = 0; i < num1; i++){
		prod = mod_sum(prod, num2, mod);
	}

	return prod;
}


int mod_eval_polynomial(int degree, int coeffs[], int mod, int x){
	long long tot = coeffs[degree];
	long long modl = (long long) mod;
	long long xl = (long long) x;
	int i;

	for(i = degree - 1; i >= 0; i--){
		tot = (tot*xl) % modl;
		tot = (tot + (long long) coeffs[i]) % modl;
	}

	return (int) tot;
}
