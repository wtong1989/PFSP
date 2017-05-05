#include "stochasticls.h"
#include "construction.h"
#include "localsearch.h"

#include <iostream>

void grasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double alpha, double timeLimit) {

    // first improvement
    // transpose move
    clock_t begin = clock(), cur;
    double t;

    // initial solution
    bestCost = rzHeuristic(instance, bestSol);

    std::vector<int> solution(instance.getNbJob()+1);

    do {

        // generate a greedy randomized initial solution
        long int cost = rzRandomizedHeuristic(instance, alpha, solution);

        // iterative improvement on this solution
        bool improve = true;
        while(improve) {
            improve = insertImprovement(instance, solution, cost, false);
        }

        // update best solution found so far
        if(cost < bestCost) {
            bestCost = cost;
            bestSol = solution;
        }

        cur = clock();
        t = ((double)(cur - begin) / CLOCKS_PER_SEC)*1000.;

    } while(t < timeLimit);

}
