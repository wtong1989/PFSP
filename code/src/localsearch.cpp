#include "localsearch.h"
#include "construction.h"
#include <cmath>
#include <iostream>

using namespace std;

/* transpose move */
void transpose(vector<int>& sol, int pos) {
  int temp = sol.at(pos);

  int next = (sol.size() == pos+1 ? 1 : pos+1);

  sol.at(pos) = sol.at(next);
  sol.at(next) = temp;
}

/* exchange move */
void exchange(vector<int>& sol, int elt1, int elt2) {
    int temp = sol.at(elt1);
    sol.at(elt1) = sol.at(elt2);
    sol.at(elt2) = temp;
}

/* insert move */
void insert(vector<int>& sol, int elt, int pos) {

    int value = sol.at(elt);

    if(pos > elt) {
        for(int i = elt; i < pos; i++) {
            sol.at(i) = sol.at(i+1);
        }
    } else {
        for(int i = elt; i > pos; i--) {
            sol.at(i) = sol.at(i-1);
        }
    }
    sol.at(pos) = value;

}

/* transpose improvement */
bool transposeImprovement(PfspInstance& instance, vector<int>& sol, long int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPos = -1;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob()-1; i++) {

        transpose(sol, i);
        newCost = instance.computePartialWCTN(sol, i);

        if(bestImp) {
            if(i == 1 || newCost < bestNewCost) {
                bestNewCost = newCost;
                bestPos = i;
            }
            transpose(sol, i); // move is canceled
        } else {
            if(newCost < cost) {
                // first improvement, the move is applied directly
                cost = instance.computePartialWCT(sol, i);
                improve = true;
            } else {
                transpose(sol, i); // no improvement, the move is canceled
            }
        }
    }

    if(bestImp && bestNewCost < cost) {
        improve = true;
        transpose(sol, bestPos);
        cost = instance.computePartialWCT(sol, bestPos);
    }

    return improve;
}

/* exchange improvement */
bool exchangeImprovement(PfspInstance& instance, vector<int>& sol, long int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPosA = -1, bestPosB = -1;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob()-1; i++) {

        for(int j = i+1; j <= instance.getNbJob(); j++) {

            exchange(sol, i, j);
            newCost = instance.computePartialWCTN(sol, i);

            if(bestImp) {
                if((i == 1 && j == 2) || newCost < bestNewCost) {
                    bestNewCost = newCost;
                    bestPosA = i;
                    bestPosB = j;
                }
                exchange(sol, i, j); // move is canceled
            } else {
                if(newCost < cost) {
                    // first improvement, the move is applied directly
                    instance.computePartialWCT(sol, i);
                    improve = true;
                    cost = newCost;
                } else {
                    exchange(sol, i, j); // move is canceled
                }
            }
        }
    }

    // in best improvement, we jump to the best solution found
    if(bestImp && bestNewCost < cost) {
        improve = true;
        exchange(sol, bestPosA, bestPosB);
        cost = instance.computePartialWCT(sol, bestPosA);
    }

    return improve;

}

/* insert improvement */
bool insertImprovement(PfspInstance& instance, vector<int>& sol, long int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPosA = -1, bestPosB = -1;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob(); i++) {

        // reset indicates if we should come back to the initial solution, if there is no improvement
        bool reset = true;

        // the element is first inserted in the first position
        if(i > 1) {
            insert(sol, i, 1);

            // the completion time matrix is entirely computed
            newCost = instance.computePartialWCT(sol, 1);
            if(bestImp) {
                if(newCost < bestNewCost) {
                    bestNewCost = newCost;
                    bestPosA = i;
                    bestPosB = 1;
                }
            } else {
                if(newCost < cost) {
                    improve = true;
                    reset = false;
                    bestPosB = 1;
                    cost = newCost;
                }
            }
        }

        // then all other positions are tried with transpose moves
        for(int j = 2; j <= instance.getNbJob(); j++) {

            transpose(sol, j-1);

            // the completion time matrix is modified only starting from the last change
            newCost = instance.computePartialWCT(sol, min(i, j-1));

            if(bestImp) {
                if((j == 2 && i == 1) || newCost < bestNewCost) {
                    bestNewCost = newCost;
                    bestPosA = i;
                    bestPosB = j;
                }
            } else {
                if(newCost < cost) {
                    // first improvement, the move is applied directly
                    // cost = instance.computePartialWCT(sol, min(i, j-1));
                    cost = instance.computePartialWCT(sol, 1);

                    improve = true;
                    reset = false;
                    bestPosB = j;
                }
            }
        }

        if(reset || bestImp) {
            if(i < instance.getNbJob()) {
                // cancel the move, reset the element to the initial position
                insert(sol, instance.getNbJob(), i);
            }
        } else {
            // go back to the best position found
            insert(sol, instance.getNbJob(), bestPosB);
        }
    }

    if(bestImp && bestNewCost < cost) {
        improve = true;
        insert(sol, bestPosA, bestPosB);
        cost = bestNewCost;
    }

    // the completion time matrix has been modified
    cost = instance.computePartialWCT(sol, 1);

    return improve;
}

/* Improve the solution */
void iterativeImprovement(PfspInstance& instance, vector< int > & sol, long int& cost, parameters& param) {

  bool improvement = true;

  int bestCost; // best cost so far
  int aMin, bMin; // changes in the solution

  // init
  if(param.rz) {
      cost = rzHeuristic(instance, sol);
  } else {
      randomPermutation(instance.getNbJob(), sol);
      cost = instance.computePartialWCT(sol, 1);
  }

  // step
  while(improvement) {

    if(param.neighborhood == 0) {
        improvement = transposeImprovement(instance, sol, cost, param.bestImp);
    } else if(param.neighborhood == 1) {
        improvement = exchangeImprovement(instance, sol, cost, param.bestImp);
    } else {
        improvement = insertImprovement(instance, sol, cost, param.bestImp);
    }

  }

}

/* Variable neighbourhood Descent heuristic */
void VNDHeuristic(PfspInstance& instance, vector< int > & sol, long int& cost, parameters& param) {

    // initialization
    if(param.rz) {
        cost = rzHeuristic(instance, sol);
    } else {
        randomPermutation(instance.getNbJob(), sol);
        cost = instance.computePartialWCT(sol, 1);
    }

    int neighbourhood = 1;
    bool improvement;

    do {

        improvement = false;

        // iterative improvement with a first improvement as pivoting rule
        // neighbourhood depends on the parameter and the neighbourhood variable
        if(neighbourhood == 1) {
            improvement = transposeImprovement(instance, sol, cost, false);
        } else if(neighbourhood == 2) {
            if(param.order) {
                improvement = insertImprovement(instance, sol, cost, false);
            } else {
                improvement = exchangeImprovement(instance, sol, cost, false);
            }
        } else if(neighbourhood == 3) {
            if(param.order) {
                improvement = exchangeImprovement(instance, sol, cost, false);
            } else {
                improvement = insertImprovement(instance, sol, cost, false);
            }
        }

        // if there is an improvement, go back to the first neighbourhood
        if(improvement) {
            neighbourhood = 1;
        } else {
            neighbourhood ++;
        }

        // the algorithm stops when no improvement has been found in the last neighborhood
    } while(neighbourhood < 4);

}

/* transpose improvement with Metropolis condition */
bool transposeImprovementMetro(PfspInstance& instance, std::vector<int>& sol, long int& cost, double T) {

    long newCost;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob()-1; i++) {

        transpose(sol, i);
        newCost = instance.computePartialWCTN(sol, i);

        bool accept = false;
        if(newCost < cost) {
            accept = true;
        } else {
            double delta = cost-newCost;
            accept = (rndNumber() <= exp(delta/T));
        }

        if(accept) {
            // first improvement, the move is applied directly
            cost = instance.computePartialWCT(sol, i);
            // long int cost2 = instance.computePartialWCT(sol, 1);
            // cout << cost << " vs " << cost2 << endl;
            // cout << " i: " << i << endl;
            // cost = cost2;
            improve = true;
        } else {
            transpose(sol, i); // no improvement, the move is canceled
        }

    }

    return improve;

}

/* insert improvement with Metropolis condition*/
bool insertImprovementMetro(PfspInstance& instance, std::vector<int>& sol, long int& cost, double T) {

    long int bestNewCost, newCost;
    int bestPos = -1;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob(); i++) {

        // reset indicates if we should come back to the initial solution, if there is no improvement
        bool reset = true;

        // the element is first inserted in the first position
        if(i > 1) {
            insert(sol, i, 1);

            // the completion time matrix is entirely computed
            newCost = instance.computePartialWCT(sol, 1);

            // compute the metropolis condition
            bool accept = false;
            if(newCost < cost) {
                accept = true;
            } else {
                double delta = cost-newCost;
                accept = (rndNumber() <= exp(delta/T));
            }

            // improvement applied directly
            if(accept) {
                improve = true;
                reset = false;
                bestPos = 1;
                cost = newCost;
            }

        }

        // then all other positions are tried with transpose moves
        for(int j = 2; j <= instance.getNbJob(); j++) {

            transpose(sol, j-1);

            // the completion time matrix is modified only starting from the last change
            newCost = instance.computePartialWCT(sol, min(i, j-1));

            // compute the metropolis condition
            bool accept = false;
            if(newCost < cost) {
                accept = true;
            } else {
                double delta = cost-newCost;
                accept = (rndNumber() <= exp(delta/T));
            }

            if(accept) {
                // first improvement, the move is applied directly
                cost = instance.computePartialWCT(sol, 1);

                improve = true;
                reset = false;
                bestPos = j;
            }

        }

        if(reset) {
            if(i < instance.getNbJob()) {
                // cancel the move, reset the element to the initial position
                insert(sol, instance.getNbJob(), i);
            }
        } else {
            // go back to the best position found
            insert(sol, instance.getNbJob(), bestPos);
        }
    }


    // the completion time matrix has been modified
    cost = instance.computePartialWCT(sol, 1);

    return improve;

}
