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
void rzHeuristic(PfspInstance& instance, vector<int>& sol) {

    vector<strSort> weightedSum(instance.getNbJob());

    for(int i = 1; i <= instance.getNbJob(); i++) {
        weightedSum.at(i-1).job = i;
        weightedSum.at(i-1).wSum = 0;
        for(int j = 1; j <= instance.getNbMac(); j++) {
            weightedSum.at(i-1).wSum += instance.getTime(i, j);
        }
        weightedSum.at(i-1).wSum /= instance.getPriority(i);
    }
    sort(weightedSum.begin(), weightedSum.end(), strSort());

    for(int i = 0; i < weightedSum.size(); i++) {
        cout << weightedSum.at(i).wSum << "  ";
    }
    cout << endl;


    sol.at(1) = weightedSum.at(0).job;
    for(int i = 2; i <= instance.getNbJob(); i++) {

        long int bestCost = -1;
        long int bestPos = -1;

        for(int j = i; j >= 1; j--) {

            sol.at(j) = weightedSum.at(i-1).job;
            // compute partial score


        }

    }


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

    if(bestImp && bestNewCost < bestImp) {
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
                }
            }
        }
    }

    if(bestImp && bestNewCost < bestImp) {
        improve = true;
        exchange(sol, bestPosA, bestPosB);
        cost = instance.computePartialWCT(sol, bestPosA);
    }

    return improve;

}

/* transpose improvement */
bool insertImprovement(PfspInstance& instance, vector<int>& sol, long int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPosA = -1, bestPosB = -1;
    bool improve = false;

    cout << "begining: " << endl;
    displaySolution(sol);
    cout << "cost beg: " << cost << endl;

    for(int i = 1; i <= instance.getNbJob(); i++) {

        bool reset = true;
        if(i > 1) {
            insert(sol, i, 1);
            newCost = instance.computePartialWCT(sol, 1);
            if(bestImp) {
                if(i == 2 || newCost < bestNewCost) {
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

        for(int j = 2; j <= instance.getNbJob(); j++) {

            transpose(sol, j-1);

            // newCost = instance.computePartialWCT(sol, min(i, j-1));
            newCost = instance.computePartialWCT(sol, 1);

            // cout << "newCost: " << newCost << endl;
            // cout << "newCost2: " << instance.computePartialWCTN(sol, 1) << endl;

            if(bestImp) {
                if(j == 2 || newCost < bestNewCost) {
                    bestNewCost = newCost;
                    bestPosA = i;
                    bestPosB = j;
                }
            } else {
                if(newCost < cost) {
                    // first improvement, the move is applied directly
                    // cost = instance.computePartialWCT(sol, min(i, j-1));
                    cost = instance.computePartialWCT(sol, 1);

                    // displaySolution(sol);

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
            // displaySolution(sol);
        }
    }

    if(bestImp && bestNewCost < cost) {
        improve = true;
        cout << "sol before: " << endl;
        displaySolution(sol);
        insert(sol, bestPosA, bestPosB);
        cout << "sol after: " << endl;
        displaySolution(sol);
        cost = bestNewCost;
        cout << "improvement: " << cost << endl;
    }

    cout << "cost end: " << cost << endl;
    cost = instance.computePartialWCT(sol, 1);
    cout << "newCost: " << newCost << endl;

    cout << "end: " << endl;
    displaySolution(sol);
    cout << "improve : " << improve << " " << endl;

    return improve;
}

/* Improve the solution */
void iterativeImprovement(PfspInstance& instance, vector< int > & sol, long int& cost, bool bestImp, int neighborhood, bool rhHeuristic) {

  bool improvement = true;

  int bestCost; // best cost so far
  int aMin, bMin; // changes in the solution

  // init
  /*if(rzHeuristic) {

  } else {
      randomPermutation(instance.getNbJob(), sol);
  }*/
  displaySolution(sol);
  bestCost = instance.computePartialWCT(sol, 1);

  // step
  while(improvement) {

    if(neighborhood == 0) {
        improvement = transposeImprovement(instance, sol, cost, bestImp);
    } else if(neighborhood == 1) {
        improvement = exchangeImprovement(instance, sol, cost, bestImp);
    } else {
        improvement = insertImprovement(instance, sol, cost, bestImp);
    }

    cost = instance.computePartialWCT(sol, 1);
    cout << cost << endl;
  }

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

  /* initialize random seed: */
  srand ( time(NULL) );

  /* Create instance object */
  PfspInstance instance;

  /* Read data from file */
  if (! instance.readDataFromFile(argv[1]) )
    return 1;

  /* Create a vector of int to represent the solution
    WARNING: By convention, we store the jobs starting from index 1,
             thus the size nbJob + 1. */
  vector< int > solution ( instance.getNbJob()+ 1 );

  /* Fill the vector with a random permutation */
  randomPermutation(instance.getNbJob(), solution);

  cout << "Random solution: " ;
  for (i = 1; i <= instance.getNbJob(); ++i)
    cout << solution[i] << " " ;
  cout << endl;

  /* Compute the TWT of this solution */
  totalWeightedTardiness = instance.computeWCT(solution);
  cout << "Total weighted completion time: " << totalWeightedTardiness << endl;

  // cout << "test : " << instance.computePartialWCT(solution, 1) << endl;
  // cout << "test2 : " << instance.computePartialWCT(solution, 1) << endl;
  // cout << "test3 : " << instance.computePartialWCTN(solution, 3) << endl;

  // rzHeuristic(instance, solution);

  for(int i = 1; i < solution.size(); i++) {
      solution.at(i) = i;
  }

  iterativeImprovement(instance, solution, totalWeightedTardiness, true, 2, false);
  cout << endl << totalWeightedTardiness << endl;
  cout << instance.computeWCT(solution) << endl;
  cout << instance.computePartialWCTN(solution, 1) << endl;

  //transpose(solution, 2);
  //displaySolution(solution);

  return 0;
}
