#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "pfspinstance.h"
#include "utils.h"

/* transpose move */
void transpose(std::vector<int>& sol, int pos);

/* exchange move */
void exchange(std::vector<int>& sol, int elt1, int elt2);

/* insert move */
void insert(std::vector<int>& sol, int elt, int pos);

/* transpose improvement */
bool transposeImprovement(PfspInstance& instance, std::vector<int>& sol, long int& cost, bool bestImp);

/* exchange improvement */
bool exchangeImprovement(PfspInstance& instance, std::vector<int>& sol, long int& cost, bool bestImp);

/* insert improvement */
bool insertImprovement(PfspInstance& instance, std::vector<int>& sol, long int& cost, bool bestImp);

/* Improve the solution */
void iterativeImprovement(PfspInstance& instance, std::vector< int > & sol, long int& cost, parameters& param);

/* Variable neighbourhood Descent heuristic */
void VNDHeuristic(PfspInstance& instance, std::vector< int > & sol, long int& cost, parameters& param);

#endif
