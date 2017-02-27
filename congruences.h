#ifndef H_CONGRUENCES
#define H_CONGRUENCES
int chinese_remainder_solution(int numOfEquations, int long[], long mods[]);
int * solve_congruence(int funcDegree, long funcCoeffs[], long mod);
int * brute_force_congruence(int degree, long coeffs[], long primeMod);
#endif
