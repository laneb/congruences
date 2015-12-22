#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_PRIME 2

int * primes(int n);
int * primes_upto(int max);

static void expand_prime_list(int length);
static void expand_prime_list_upto(int max);
static int any_divisors(int * div_list, int length, int num);

static int * PRIME_LIST = NULL;
static int PRIME_LIST_LENGTH = 0;
static int PRIME_LIST_MAX_LENGTH = 0;
static int NEXT_INTEGER = FIRST_PRIME;


int * primes(int length){
	int * rtrnList = calloc(length, sizeof(int));

	expand_prime_list(length);

	if(rtrnList != NULL){
		memcpy(rtrnList, PRIME_LIST, length*sizeof(int));
	}

	return rtrnList;
}


int * primes_upto(int max){
	int * rtrnList;
	int i;

	expand_prime_list_upto(max);

	for(i = PRIME_LIST_LENGTH; i > 0; i--){
		if(PRIME_LIST[i-1] <= max){
			rtrnList = calloc(i+1, sizeof(int));
			rtrnList[0] = i;
			memcpy(rtrnList+1, PRIME_LIST, i*sizeof(int));
			return rtrnList;
		}
	}

	return NULL;
}


static void expand_prime_list(int length){

	if(PRIME_LIST_MAX_LENGTH < length){
		PRIME_LIST_MAX_LENGTH = 2*length;
		PRIME_LIST = realloc(PRIME_LIST, PRIME_LIST_MAX_LENGTH*sizeof(int));
	}

	while(PRIME_LIST_LENGTH < length){
		if( !any_divisors(PRIME_LIST, PRIME_LIST_LENGTH, NEXT_INTEGER) ){
			PRIME_LIST[PRIME_LIST_LENGTH++] = NEXT_INTEGER;
		}

		NEXT_INTEGER++;
	}

}


static void expand_prime_list_upto(int max){
	
	if(PRIME_LIST == NULL){
		//TODO: find better heuristic limit on memory necessary to allocate
		PRIME_LIST_MAX_LENGTH = (max/2) + 1;
		PRIME_LIST = calloc(PRIME_LIST_MAX_LENGTH, sizeof(int));
	}
	
	while(NEXT_INTEGER <= max){
		if( !any_divisors(PRIME_LIST, PRIME_LIST_LENGTH, NEXT_INTEGER) ){
			if( PRIME_LIST_MAX_LENGTH <= PRIME_LIST_LENGTH){
				PRIME_LIST_MAX_LENGTH *= 2;
				PRIME_LIST = realloc(PRIME_LIST, PRIME_LIST_MAX_LENGTH*sizeof(int));
			}

			PRIME_LIST[PRIME_LIST_LENGTH++] = NEXT_INTEGER;
		}

		NEXT_INTEGER++;
	}
}


static int any_divisors(int * div_list, int length, int num){
	int i;
	for(i = 0; i < length; i++){
		if(num % div_list[i] == 0){
			return 1;
		}
	}

	return 0;
}
