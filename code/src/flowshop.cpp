/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "pfspinstance.h"
#include <algorithm>

using namespace std;

struct strSort {
    long int job;
    long int wSum;

    bool operator()(strSort& a, strSort& b) {
        return a.wSum < b.wSum;
    }

} sortObj;

struct parameters {
    bool bestImp;
    bool rz;
    int neighborhood;

    bool order; // for vnd heuristic
    bool vnd;
} param;

/* display the solution */
void displaySolution(vector<int>& sol) {
  for(int i = 1; i < sol.size(); i++) {
    cout << sol.at(i) << " ";
  }
  cout << endl;
}

int generateRndPosition(int min, int max)
{
  return ( rand() % max + min );
}

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


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void randomPermutation(int nbJobs, vector< int > & sol)
{
  vector<bool> alreadyTaken(nbJobs+1, false); // nbJobs elements with value false
  vector<int > choosenNumber(nbJobs+1, 0);

  int nbj;
  int rnd, i, j, nbFalse;

  nbj = 0;
  for (i = nbJobs; i >= 1; --i)
  {
    rnd = generateRndPosition(1, i);
    nbFalse = 0;

    /* find the rndth cell with value = false : */
    for (j = 1; nbFalse < rnd; ++j)
      if ( ! alreadyTaken[j] )
        ++nbFalse;
    --j;

    sol[j] = i;

    ++nbj;
    choosenNumber[nbj] = j;

    alreadyTaken[j] = true;
  }
}

/* simplified rz heuristic */
long int rzHeuristic(PfspInstance& instance, vector<int>& sol) {

    long int res = 0;
    vector<strSort> weightedSum(instance.getNbJob());

    // sort the jobs according to the weighted sums
    for(int i = 1; i <= instance.getNbJob(); i++) {
        weightedSum.at(i-1).job = i;
        weightedSum.at(i-1).wSum = 0;
        for(int j = 1; j <= instance.getNbMac(); j++) {
            weightedSum.at(i-1).wSum += instance.getTime(i, j);
        }
        weightedSum.at(i-1).wSum /= instance.getPriority(i);
    }
    sort(weightedSum.begin(), weightedSum.end(), strSort());

    sol.at(1) = weightedSum.at(0).job;

    // start computing the matrix
    instance.computePartialWCT(sol, 1, 1);

    for(int i = 2; i <= instance.getNbJob(); i++) {

        // for each element, determine the best insertion position
        long int bestCost;
        long int bestPos;

        // first position tested : end of the solution
        sol.at(i) = weightedSum.at(i-1).job;
        bestCost = instance.computePartialWCT(sol, i, i);
        bestPos = i;

        for(int j = i-1; j >= 1; j--) {

            transpose(sol, j);
            // compute partial score
            long int cost = instance.computePartialWCTN(sol, j, i);
            if(cost < bestCost) {
                bestCost = cost;
                bestPos = j;
            }
        }

        // the element is now in the first position, move it back to the best pos found
        insert(sol, 1, bestPos);
        res = instance.computePartialWCT(sol, bestPos, i);
    }

    return res;
}


/* transpose improvement */
bool transposeImprovement(PfspInstance& instance, vector<int>& sol, long int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPos = -1;
    bool improve = false;

    for(int i = 1; i <= instance.getNbJob(); i++) {

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

// setup the param data structure with the command line parameters
/*----------------------------------------------------------------------------*/
void extractParameters(char* cmd[], int nb, parameters& param) {

    string str;
    for(int i = 2; i < nb; i++) {
        str += cmd[i];
    }

    if(str.find("--vnd") != string::npos) {
        param.vnd = true;
    } else {
        param.vnd = false;
    }

    if(param.vnd && str.find("--exfirst") != string::npos) {
        param.order = true;
    } else if(param.vnd && str.find("--insfirst") != string::npos) {
        param.order = false;
    } else {
        param.order = false;
    }

    if(str.find("--srz") != string::npos) {
        param.rz = true;
    } else if(str.find("--random") != string::npos) {
        param.rz = false;
    } else {
        param.rz = false;
    }

    if(str.find("--transpose") != string::npos) {
        param.neighborhood = 0;
    } else if(str.find("--exchange") != string::npos) {
        param.neighborhood = 1;
    } else if (str.find("--insert") != string::npos){
        param.neighborhood = 2;
    } else {
        param.neighborhood = 0;
    }

    if(str.find("--first") != string::npos) {
        param.bestImp = false;
    } else if(str.find("--best") != string::npos){
        param.bestImp = true;
    } else {
        param.bestImp = false;
    }

    cout << endl << "options: " << endl;
    if(param.vnd) {
        cout << "- Variable Neighbourhood Descent heuristic" << endl;
    }

    cout << "- initialization: " << (param.rz ? "siplified rz" : "random permutation") << endl;

    if(!param.vnd) {
        cout << "- pivoting rule: " << (param.bestImp ? "best improvement" : "first improvement") << endl;
        cout << "- neighborhood: ";
        if(param.neighborhood == 0) {
            cout << "transpose";
        } else if(param.neighborhood == 1) {
            cout << "exchange";
        } else {
            cout << "insert";
        }
    } else {
        cout << "-neighbourhood order: " << endl;
        cout << "\ttranspose ; ";
        if(param.order) {
            cout << "exchange ; insert" << endl;
        } else {
            cout << "insert ; exchange" << endl;
        }
    }

    cout << endl << endl;

}

/***********************************************************************/

int main(int argc, char *argv[])
{
  int i;
  long int totalWeightedTardiness;


  if (argc == 1)
  {
    cout << "Usage: ./flowshopWCT <instance_file>" << endl;
    return 0;
  }

  parameters param;
  if(argc > 2) {
      extractParameters(argv, argc, param);
  }

  /* initialize random seed: */
  srand ( 42 );

  /* Create instance object */
  PfspInstance instance;

  /* Read data from file */
  if (! instance.readDataFromFile(argv[1]) )
    return 1;

  /* Create a vector of int to represent the solution
    WARNING: By convention, we store the jobs starting from index 1,
             thus the size nbJob + 1. */
  vector< int > solution ( instance.getNbJob()+ 1 );

  clock_t begin, end;

  begin = clock();

  // apply the chosen heuristic
  if(param.vnd) {
      VNDHeuristic(instance, solution, totalWeightedTardiness, param);
  } else {
      iterativeImprovement(instance, solution, totalWeightedTardiness, param);
  }

  end = clock();

  cout << endl << "Best: " << totalWeightedTardiness << endl;

  double t = (double)(end - begin) / CLOCKS_PER_SEC;
  t *= 1000.;

  cout << "Time: " << t << endl;

  return 0;
}
