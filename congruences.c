#include "arith_utils.c"


int * brute_force_congruence(int degree, int coeffs[], int primeMod);
int chinese_remainder_solution(int numOfEquations, int scals[], int mods[]);
int * linear_diophantine_solution(int numOfTerms, int coeffs[], int scal);
int * solve_prime_power_congruence(int degree, int coeffs[], int prime, int power);
int * solve_system_of_congruence_sets(int numOfSets, int ** sets, int mods[]);
int * solve_congruence(int funcDegree, int funcCoeffs[], int mod);
int * solve_system_of_congruences(int numOfFuncs, int * funcDegrees, int ** funcCoeffs, int * mods);


int chinese_remainder_solution(int numberOfEquations, int scals[], int mods[]){
	int i;
	int x = 0;
	int m = mods[0];
	int modCoeff;

	for(i=1; i<numberOfEquations; i++){
		m *= mods[i];
	}

	for(i=0; i<numberOfEquations; i++){
		modCoeff = m/mods[i];
		x += modCoeff*mod_inv(modCoeff, mods[i])*scals[i];
	}

	return x % m;
}


int * brute_force_congruence(int degree, int coeffs[], int primeMod){
	//assumes a prime modulus. split congruences of composite modulus into systems of congrueneces
	//of prime modulus and/or apply the lifting theorem to make use of this function
	//solve a0x^n + a1x^n-1... = 0 (mod mod) where n is the order a0, a1, ... are coeffieicients
	int * solutionList = calloc(degree+1, sizeof(int));
	int * solutions = solutionList+1;
	int numberOfSolutions = 0;
	int x;

	for(x = 0; x < primeMod && numberOfSolutions <= degree; x++){
		if(mod_eval_polynomial(degree, coeffs, primeMod, x) == 0){
			solutions[numberOfSolutions++] = x;
		}
	}

	*solutionList = numberOfSolutions;

	return solutionList;
}


int * solve_prime_power_congruence(int funcDegree, int funcCoeffs[], int prime, int power){

	int * baseSolutionList;
	int numOfBaseSolutions;
	int * baseSolutions;

	int * liftedSolutions;
	int numOfLiftedSolutions;

	int derivDegree;
	int * derivCoeffs;
	int deriv;
	long int divFunc;

	int j, t;
	int currentMod;

	if(power == 1){
		return brute_force_congruence(funcDegree, funcCoeffs, prime);
	}

	baseSolutionList = solve_prime_power_congruence(funcDegree, funcCoeffs, prime, power-1);
	numOfBaseSolutions = *baseSolutionList;
	baseSolutions = baseSolutionList+1;

	liftedSolutions = calloc(prime*numOfBaseSolutions+1, sizeof(int));
	numOfLiftedSolutions = 0;

	derivDegree = funcDegree-1;
	derivCoeffs = calloc(derivDegree+1, sizeof(int));

	currentMod = prime;
	for(j = 1; j < power; j++){
		currentMod *= prime;
	}

	for(j = 0; j <= derivDegree; j++){
		derivCoeffs[j] = funcCoeffs[j+1]*(j+1);
	}


	for(j = 0; j < numOfBaseSolutions; j++){
		deriv = mod_eval_polynomial(derivDegree, derivCoeffs, prime, baseSolutions[j]);
		divFunc = (eval_polynomial(funcDegree, funcCoeffs, baseSolutions[j]) / (currentMod/prime)) % prime;

		if(deriv % prime != 0){
			t = (-divFunc*mod_inv(deriv, prime) % prime) + prime;
			liftedSolutions[++numOfLiftedSolutions] = baseSolutions[j] + t*prime;
		}

		else if(divFunc % prime == 0){
			for(t = 1; t <= prime; t++){
				liftedSolutions[++numOfLiftedSolutions] = baseSolutions[j] + t*prime;
			}
		}
	}

	*liftedSolutions = numOfLiftedSolutions;

	free(derivCoeffs);
	free(baseSolutionList);

	return liftedSolutions;
}


int * solve_system_of_congruence_sets(int numOfSets, int * * sets, int mods[]){
	//heuristic approach: apply CRT individually

	int * set1;
	int * set2;
	int set1Size;
	int set2Size;

	int congruencePair[2];

	int * generalSolutionList;
	int * localSolutionList;
	int * localSolutions;
	int numOfSolutions;

	int i;
	int j;


	if(numOfSets == 1){
		return sets[0];
	}


	if(sets[0][0] == 0){
		generalSolutionList = malloc( sizeof(int) );
		generalSolutionList[0] = 0;
		return generalSolutionList;
	}


	set1 = sets[0]+1;
	set2 = sets[1]+1;
	set1Size = sets[0][0];
	set2Size = sets[1][0];
	numOfSolutions = set1Size*set2Size;
	localSolutionList = calloc(numOfSolutions+1, sizeof(int));
	localSolutions = localSolutionList+1;
	*localSolutionList = numOfSolutions;

	for(i = 0, numOfSolutions = 0; i < set1Size; i++){
		congruencePair[0] = set1[i];

		for(j = 0; j < set2Size; j++){
			congruencePair[1] = set2[j];
			localSolutions[numOfSolutions++] = chinese_remainder_solution(2, congruencePair, mods);
		}
	}


	sets[1] = localSolutionList;
	mods[1] *= mods[0];
	generalSolutionList = solve_system_of_congruence_sets(numOfSets-1, sets+1, mods+1);


	return generalSolutionList;
}



int * solve_congruence(int funcDegree, int funcCoeffs[], int mod){
	int * solutionList;

	int * modFactorList = prime_factors(mod);
	int numOfModFactors = *modFactorList;
	int * modFactors = modFactorList+1;

	int * * primePowerSolutions = calloc(numOfModFactors, sizeof(int *));
	int * primePowers = calloc(numOfModFactors, sizeof(int));


	int power;
	int i;


	for(i = 0; i < numOfModFactors; i++){
		primePowers[i] = modFactors[i]; 
		power = 1;
		
		while(mod % (primePowers[i]*modFactors[i]) == 0){
			primePowers[i] *= modFactors[i];
			power++;
		}

		primePowerSolutions[i] = solve_prime_power_congruence(funcDegree, funcCoeffs, modFactors[i], power);
	}


	solutionList = solve_system_of_congruence_sets(numOfModFactors, primePowerSolutions, primePowers);


	free(modFactorList);

	return solutionList;
}


int * solve_system_of_congruences(int numOfFuncs, int * funcDegrees, int ** funcCoeffs, int * mods){
	int i;
	int * * funcSolutionSets = calloc(numOfFuncs, sizeof(int *));

	for(i=0; i<numOfFuncs; i++){
		funcSolutionSets[i] = solve_congruence(funcDegrees[i], funcCoeffs[i], mods[i]);
	}

	return solve_system_of_congruence_sets(numOfFuncs, funcSolutionSets, mods);
}	

