#ifndef STOCHASTIC_L_S_H
#define STOCHASTIC_L_S_H

#include "pfspinstance.h"

// grasp metaheuristic
void grasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double alpha, double timeLimit);

// pick a random alpha according to the probabilities
int pickAlpha(const std::vector<double>& probas);

// update the alpha's probabilities
void updateProbas(std::vector<double>& probas, long int& zBest, long int& zWorst, std::vector<int>& freqAlpha, std::vector<double>& sumAlpha);

// reactive grasp metaheuristic
void reactiveGrasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, int m, int Nalpha, double timeLimit);

// simulated annealing metaheuristic
void simulatedAnnealing(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double T0, double alpha, int n, double timeLimit);

#endif
