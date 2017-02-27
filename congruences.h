#ifndef H_CONGRUENCES
#define H_CONGRUENCES
long chinese_remainder_solution(int numOfEquations, int long[], long mods[]);
long * solve_congruence(int funcDegree, long funcCoeffs[], long mod);
long * brute_force_congruence(int degree, long coeffs[], long primeMod);
#endif
