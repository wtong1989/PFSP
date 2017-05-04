#ifndef UTILS_H
#define UTILS_H

#include <vector>

struct parameters {
    bool bestImp;
    bool rz;
    int neighborhood;

    bool order; // for vnd heuristic
    bool vnd;
} params;

/* display a solution */
void displaySolution(std::vector<int>& sol);

/* generate a random position */
int generateRndPosition(int min, int max);

// setup the param data structure with the command line parameters
/*----------------------------------------------------------------------------*/
void extractParameters(char* cmd[], int nb, parameters& param);

#endif
