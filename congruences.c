#include "arith_utils.h"
#include "prime_gen.h"
#include <stdlib.h>
#include <stdio.h>

static long * adjust_coeffs_to_mod(int degree, long * coeffs, long mod);
static long * solve_prime_power_congruence(int degree, long coeffs[], long prime, int power);
static long * solve_system_of_order_1_congruence_sets(int numOfSets, int * lengthsOfSets, long ** sets, long mods[]);

long chinese_remainder_solution(int numberOfEquations, long scals[], long mods[]){
  int i;
  long long x = 0;
  long m = mods[0];
  long modCoeff;

  for(i=1; i<numberOfEquations; i++){
    m *= mods[i];
  }

  for(i=0; i<numberOfEquations; i++){
    modCoeff = m/mods[i];
    x += (long long) modCoeff*mod_inv(modCoeff % mods[i], mods[i])*scals[i] % m;
    x %= m;
  }

  return x % m;
}


long * adjust_coeffs_to_mod(int degree, long * coeffs, long mod){
  long * adjustedCoeffs = calloc(degree+1, sizeof(long));
  int i;

  for(i = 0; i <= degree; i++){
    adjustedCoeffs[i] = coeffs[i] % mod;
    if(adjustedCoeffs[i] < 0){
      adjustedCoeffs[i] += mod;
    }
  }

  return adjustedCoeffs;
}


long * brute_force_congruence(int degree, long coeffs[], long primeMod){
  //assumes a prime modulus. split congruences of composite modulus into systems of congrueneces
  //of prime modulus and/or apply the lifting theorem to make use of this function
  //solve a0x^n + a1x^n-1... = 0 (mod mod) where n is the order a0, a1, ... are coeffieicients
  //also assumes positive representation of coeffs
  long * adjustedCoeffs = adjust_coeffs_to_mod(degree, coeffs, primeMod);
  long * solutionList = calloc(degree+1, sizeof(long));
  long * solutions = solutionList+1;
  int numberOfSolutions = 0;
  long x;

  #pragma omp parallel for
  for(x = 0; x < primeMod; x++){
    if(mod_eval_polynomial(degree, adjustedCoeffs, primeMod, x) == 0){
      solutions[numberOfSolutions++] = x;
    }
  }

  *solutionList = numberOfSolutions;

  free(adjustedCoeffs);

  return solutionList;
}


static long * solve_prime_power_congruence(int funcDegree, long funcCoeffs[], long prime, int power){
  long * baseSolutionList;
  int numOfBaseSolutions;
  long * baseSolutions;

  long * liftedSolutions;
  int numOfLiftedSolutions;

  long coeff;

  int derivDegree;
  long * derivCoeffs;
  long deriv;
  long func;

  int i, j, t;
  long currentMod;

  if(power == 1){
    baseSolutions = brute_force_congruence(funcDegree, funcCoeffs, prime);
    return baseSolutions;
  }

  baseSolutionList = solve_prime_power_congruence(funcDegree, funcCoeffs, prime, power-1);
  numOfBaseSolutions = *baseSolutionList;
  baseSolutions = baseSolutionList+1;

  liftedSolutions = calloc(prime*numOfBaseSolutions+1, sizeof(long));
  numOfLiftedSolutions = 0;

  derivDegree = funcDegree-1;
  derivCoeffs = calloc(derivDegree+1, sizeof(long));

  currentMod = prime;
  for(j = 1; j < power; j++){
    currentMod *= prime;
  }

  for(j = 0; j <= derivDegree; j++){
    coeff = funcCoeffs[j+1] % prime;
    if(coeff < 0){
      coeff += prime;
    }
    derivCoeffs[j] = mod_product(coeff, j+1, prime);
  }


  for(j = 0; j < numOfBaseSolutions; j++){

    deriv = mod_eval_polynomial(derivDegree, derivCoeffs, prime, baseSolutions[j]);
    func = mod_eval_polynomial(funcDegree, funcCoeffs, currentMod, baseSolutions[j]);

    if(deriv % prime != 0){
      t = ((-func / (currentMod/prime))*mod_inv(deriv, currentMod) % prime) + prime;
      liftedSolutions[++numOfLiftedSolutions] = baseSolutions[j] + t*prime;
    }

    else if(func == 0){
      for(t = 1; t <= prime; t++){
        liftedSolutions[++numOfLiftedSolutions] = baseSolutions[j] + t*(currentMod/prime);
      }
    }
  }


  *liftedSolutions = numOfLiftedSolutions;

  free(derivCoeffs);
  free(baseSolutionList);

  return liftedSolutions;
}


static long * solve_system_of_order_1_congruence_sets(int numOfSets, int * setLengths, long * * sets, long * mods){
  //allocate perumtation array
  long * divAry = calloc(numOfSets, sizeof(long));
  long * scalAry = calloc(numOfSets, sizeof(long));
  int i, j;
  int numOfSolutions;
  long * solutionAry;
  long * dest;
  int idx;

  for(i = 0, numOfSolutions = 1; i < numOfSets; i++){
    divAry[i] = numOfSolutions;
    numOfSolutions *= setLengths[i];
  }

  solutionAry = calloc(numOfSolutions+1, sizeof(long));
  solutionAry[0] = numOfSolutions;
  dest = solutionAry+1;

  for(i = 0; i < numOfSolutions; i++){
    for(j = 0; j < numOfSets; j++){
      idx = (i / divAry[j]) % setLengths[j];
      scalAry[j] = sets[j][idx];
    }

    *(dest++) = chinese_remainder_solution(numOfSets, scalAry, mods);
  }

  return solutionAry;
}

long * solve_congruence(int funcDegree, long funcCoeffs[], long mod){
  long * solutionList;

  long * modFactorList = prime_factors(mod);
  long numOfModFactors = *modFactorList;
  long * modFactors = modFactorList+1;

  long * * primePowerSolutions = calloc(numOfModFactors, sizeof(long *));
  long * primePowers = calloc(numOfModFactors, sizeof(long));
  int * primePowerSolutionLengths = calloc(numOfModFactors, sizeof(int *));

  int power;
  int i,j,k;

  for(i = 0; i < numOfModFactors; i++){
    primePowers[i] = modFactors[i];
    power = 1;

    while(mod % (primePowers[i]*modFactors[i]) == 0){
      primePowers[i] *= modFactors[i];
      power++;
    }

    primePowerSolutions[i] = solve_prime_power_congruence(funcDegree, funcCoeffs, modFactors[i], power);
    primePowerSolutionLengths[i] = *(primePowerSolutions[i]++);
  }

  solutionList = solve_system_of_order_1_congruence_sets(numOfModFactors, primePowerSolutionLengths, primePowerSolutions, primePowers);

  for(i = 0; i < numOfModFactors; i++){
    free(primePowerSolutions[i] - 1);
  }
  free(primePowerSolutionLengths);
  free(primePowerSolutions);
  free(primePowers);
  free(modFactorList);

  return solutionList;
}
