#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

typedef struct parameters {
    bool bestImp;
    bool rz;
    int neighborhood;

    bool order; // for vnd heuristic
    bool vnd;
} params;

typedef struct parameters2 {

    bool algo; // grasp or simulated annealing
    double timeLimit;
    int seed; // seed for the random generator
    int instanceName; // index of the instance string in the command line parameters

} params2;

/* display a solution */
void displaySolution(std::vector<int>& sol);

/* generate a random position */
int generateRndPosition(int min, int max);

/* generate a random number between and one */
double rndNumber();

// setup the param data structure with the command line parameters (implementation exercise 1)
/*----------------------------------------------------------------------------*/
void extractParameters(char* cmd[], int nb, parameters& param);

/* setup the param data structure with the command line parameters (implementation exercise 2)*/
void extractParameters2(char* cmd[], int nb, parameters2& param);

#endif
