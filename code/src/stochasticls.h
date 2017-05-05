#ifndef STOCHASTIC_L_S_H
#define STOCHASTIC_L_S_H

#include "pfspinstance.h"

// grasp metaheuristic
void grasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double alpha, double timeLimit);

#endif
