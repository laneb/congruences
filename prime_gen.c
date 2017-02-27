#include "prime_gen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define FIRST_PRIME 2

static void expand_prime_list_to_length(int length);
static void expand_prime_list_past(long max);
static int any_divisors(long * div_list, int length, long num);
static long least_divisor(long n);

static long * PRIME_LIST = NULL;
static int PRIME_LIST_LENGTH = 0;
static int PRIME_LIST_MAX_LENGTH = 0;
static long NEXT_INTEGER = FIRST_PRIME;


long * primes(int length){
	long * rtrn_list = calloc(length, sizeof(long));

	expand_prime_list_to_length(length);

	if(rtrn_list != NULL){
		memcpy(rtrn_list, PRIME_LIST, length*sizeof(long));
	}

	return rtrn_list;
}


long * primes_upto(long max){
	long * rtrn_list;
	int i;

	expand_prime_list_past(max);

	for(i = PRIME_LIST_LENGTH; i > 0; i--){
		if(PRIME_LIST[i-1] <= max){
			rtrn_list = calloc(i+1, sizeof(long));
			rtrn_list[0] = i;
			memcpy(rtrn_list+1, PRIME_LIST, i*sizeof(long));
			return rtrn_list;
		}
	}

	return NULL;
}


long * prime_factors(long n){
	long * rtrn_list = malloc(sizeof(long));
	int rtrn_list_length = 0;
	long least_div;

	while(n != 1){
		least_div = least_divisor(n);
		rtrn_list = realloc(rtrn_list, (rtrn_list_length+2)*sizeof(long));
		rtrn_list[++rtrn_list_length] = least_div;

		if(least_div == n){
			break;
		}

		while(n % least_div == 0){
			n /= least_div;
		}
	}

	rtrn_list[0] = rtrn_list_length;

	return rtrn_list;
}


static long least_divisor(long n){
	//Calculate maximum for least divisor (sqrt)
	long least_div_max = sqrt(n) + 1;
	int i;
	//Expand prime list up to the least divisor
	expand_prime_list_past(least_div_max);


	for(i = 0; PRIME_LIST[i] < least_div_max; i++){
		if(n % PRIME_LIST[i] == 0){
			return PRIME_LIST[i];
		}
	}

	return n;
}


static void expand_prime_list_to_length(int length){

	if(PRIME_LIST_MAX_LENGTH < length){
		PRIME_LIST_MAX_LENGTH = 2*length;
		PRIME_LIST = realloc(PRIME_LIST, PRIME_LIST_MAX_LENGTH*sizeof(long));
	}

	while(PRIME_LIST_LENGTH < length){
		if( !any_divisors(PRIME_LIST, PRIME_LIST_LENGTH, NEXT_INTEGER) ){
			PRIME_LIST[PRIME_LIST_LENGTH++] = NEXT_INTEGER;
		}

		NEXT_INTEGER++;
	}

}


static void expand_prime_list_past(long max){
	if(PRIME_LIST == NULL){
		//TODO: find better heuristic limit on memory necessary to allocate
		PRIME_LIST_MAX_LENGTH = (max/2) + 1;
		PRIME_LIST = calloc(PRIME_LIST_MAX_LENGTH, sizeof(long));
		PRIME_LIST[0] = NEXT_INTEGER++;
		PRIME_LIST_LENGTH = 1;
	}

	while(PRIME_LIST[PRIME_LIST_LENGTH-1] <= max){
		while(any_divisors(PRIME_LIST, PRIME_LIST_LENGTH, NEXT_INTEGER) ){
			NEXT_INTEGER++;
		}

		if( PRIME_LIST_MAX_LENGTH <= PRIME_LIST_LENGTH){
			PRIME_LIST_MAX_LENGTH *= 2;
			PRIME_LIST = realloc(PRIME_LIST, PRIME_LIST_MAX_LENGTH*sizeof(long));
		}

		PRIME_LIST[PRIME_LIST_LENGTH++] = NEXT_INTEGER;
	}
}


static int any_divisors(long * div_list, int length, long num){
	int i;
	for(i = 0; i < length; i++){
		if(num % div_list[i] == 0){
			return 1;
		}
	}

	return 0;
}
