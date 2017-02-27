#ifndef H_CONGRUENCES
#define H_CONGRUENCES
int chinese_remainder_solution(int numOfEquations, int scals[], int mods[]);
int * solve_congruence(int funcDegree, long funcCoeffs[], int mod);
int * brute_force_congruence(int degree, long coeffs[], int primeMod);
#endif
