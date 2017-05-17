#include "stochasticls.h"
#include "construction.h"
#include "localsearch.h"

#include <iostream>
#include <list>

using namespace std;

// grasp metaheuristic
void grasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double alpha, double timeLimit) {

    // first improvement
    // transpose move
    clock_t begin = clock(), cur;
    double t;

    // uninitalised solution
    bestCost = -1;

    std::vector<int> solution(instance.getNbJob()+1);

    std::vector<int> solution2(instance.getNbJob()+1);
    long int cost2;

    bool stepTwo;

    do {


        // generate a greedy randomized initial solution
        long int cost = rzRandomizedHeuristic(instance, alpha, solution);

        // iterative improvement on this solution
        bool improve = true;
        while(improve) {
            improve = insertImprovement(instance, solution, cost, false);
        }

        stepTwo = false;
        // update best solution found so far
        if(cost < bestCost || bestCost < 0) {
            bestCost = cost;
            bestSol = solution;
            cur = clock();
            stepTwo = true;
        } else {
            double proxi = (double)(cost-bestCost)/(double)cost;

            if(proxi <= 0.02) {
                stepTwo = true;
            }
        }

        // perform a perturbative local search
        if(stepTwo) {
            // nbStepTwo ++;
            for(int i = 0; i < 18; i++) {

                solution2 = solution;
                permurbativeStep(instance, solution2, cost2, 5);

                // iterative improvement on this solution
                bool improve = true;
                while(improve) {
                    improve = insertImprovement(instance, solution2, cost2, false);
                }

                // update best solution found so far
                if(cost2 < bestCost) {
                    bestCost = cost2;
                    bestSol = solution2;
                }

            }

        }

        cur = clock();
        t = ((double)(cur - begin) / CLOCKS_PER_SEC)*1000.;

    } while(t < timeLimit);

    // cout << "nb run grasp: " << nbGr << " ; nb step two: " << nbStepTwo << endl;

}


// pick a random alpha according to the probabilities
int pickAlpha(const std::vector<double>& probas) {

    double drawing = rndNumber();
    int k = 0;
    double sum = 0;

    while(sum < drawing) {

        sum += probas.at(k);
        if(sum < drawing) {
            k ++;
        }
    }

    return k;
}

// update the alpha's probabilities
void updateProbas(std::vector<double>& probas, long int& zBest, long int& zWorst, std::vector<int>& freqAlpha, std::vector<double>& sumAlpha) {

    double sum = 0.;

    for(int i = 0; i < probas.size(); i++) {
        double avg = sumAlpha.at(i)/(double)freqAlpha.at(i);
        probas.at(i) = (avg - (double)zWorst)/((double)zBest - (double)zWorst);
        sum += probas.at(i);
    }
    for(int i = 0; i < probas.size(); i++) {
        probas.at(i) /= sum;
    }

    zBest = -1;
    zWorst = -1;

    for(int i = 0; i < probas.size(); i++) {
        cout << probas.at(i) << " ";
    }
    cout << endl << endl;

}

// reactive grasp metaheuristic
void reactiveGrasp(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, int m, int Nalpha, double timeLimit) {

    vector<double> probas(m, 1./(double)m); // probabilities of picking each alpha
    vector<double> sumAlpha(m, 0); // average quality of a solution for a given alpha
    vector<int> freqAlpha(m, 0); // frequency of alphas

    clock_t begin = clock(), cur;
    double t;

    // uninitialised solution
    bestCost = -1;

    vector<int> solution(instance.getNbJob()+1);

    int it = 0; // current number of iteration during one reactive run
    long int bestNalpha = -1, worstNAlpha = -1; // best and worst alpha for a reactive run (after each reactive run, probas are updated)
    int k; // chosen alpha

    // TODO don't reset the averages after each reactive iteration

    do {
        // generate a greedy randomized initial solution
        k = pickAlpha(probas);
        long int cost = rzRandomizedHeuristic(instance, (double)k/(double)m, solution);

        // iterative improvement on this solution
        bool improve = true;
        while(improve) {
            improve = insertImprovement(instance, solution, cost, false);
        }

        // update the best solution found so far
        if(cost < bestCost || bestCost < 0) {
            bestCost = cost;
            bestSol = solution;
        }

        // update best and worst solution during this reactive run
        if(cost < bestNalpha || bestNalpha < 0) {
            bestNalpha = cost;
        }
        if(cost > worstNAlpha || worstNAlpha < 0) {
            worstNAlpha = cost;
        }
        sumAlpha.at(k) += cost;
        freqAlpha.at(k) ++;

        it ++;

        // end of the reactive run, update probabilities
        if(it >= Nalpha) {
            updateProbas(probas, bestNalpha, worstNAlpha, freqAlpha, sumAlpha);
            it = 0;
        }

        cur = clock();
        t = ((double)(cur - begin) / CLOCKS_PER_SEC)*1000.;

    } while(t < timeLimit);

}

// simulated annealing metaheuristic
void simulatedAnnealing(PfspInstance& instance, std::vector<int>& bestSol, long int& bestCost, double T0, double alpha, int n, double timeLimit) {

    // initial solution
    bestCost = rzHeuristic(instance, bestSol);

    vector<int> sol = bestSol;
    long int cost = bestCost;

    // time measure
    clock_t begin = clock(), cur;
    double t;

    cur = clock();

    // temperature
    double T = T0;

    // number of iteration
    int it = 0;

    int it2 = 0;
    bool improve = false;

    do {

        // perform local search
        // insertImprovementMetro(instance, sol, cost, T);
        transposeImprovementMetro(instance, sol, cost, T);

        // update best solution if necessary
        if(cost < bestCost) {
            bestCost = cost;
            bestSol = sol;
            improve = true;
            cur = clock();
        }

        // update temperature
        it ++;
        if(it >= n) {
            it = 0;
            T = T*alpha;

            if(!improve) {
                if(T <= 100) {
                    T = T0;
                }
            } else {
                improve = false;
            }
        }

        it2 ++;

        cur = clock();
        t = ((double)(cur - begin) / CLOCKS_PER_SEC)*1000.;

    } while(t < timeLimit);


}

// perturbative step on a solution: perform n random insert moves
void permurbativeStep(PfspInstance& instance, std::vector<int>& sol, long int& cost, int n) {

    for(int i = 0; i < n; i++) {
        int elt = generateRndPosition(1, instance.getNbJob());
        int pos = generateRndPosition(1, instance.getNbJob());
        insert(sol, elt, pos);
    }
    cost = instance.computePartialWCT(sol, 1, instance.getNbJob());

}
