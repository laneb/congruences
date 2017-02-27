#include <stdio.h>
#include <stdlib.h>
#include "prime_gen.h"
#include "arith_utils.h"

//Expects 0 <= x,y < mod
long mod_sum(long x, long y, long mod){
	if(y >= mod - x){
		return y - (mod - x);
	}

	else{
		return y + x;
	}
}


long mod_inv(long n, long mod){
	long y, a;

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


long mod_product(long num1, long num2, long mod){
	long prod = 0;
	long i;

	for(i = 0; i < num1; i++){
		prod = mod_sum(prod, num2, mod);
	}

	return prod;
}


long mod_eval_polynomial(int degree, long coeffs[], long mod, long x){
	long long tot = coeffs[degree];
	int i;

	for(i = degree - 1; i >= 0; i--){
		tot = ( (long long) tot*x ) % mod;
		tot = ( (long long) tot + coeffs[i] ) % mod;
	}

	return (long) tot;
}
