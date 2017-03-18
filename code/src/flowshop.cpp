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

using namespace std;


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

    for(int i = elt; i < pos; i++) {
        sol.at(i) = sol.at(i+1);
    }
    sol.at(pos) = value;

}


/* transpose improvement */
bool transposeImprovement(PfspInstance& instance, vector<int>& sol, int& cost, bool bestImp) {

    long int bestNewCost, newCost;
    int bestPos = -1;
    bool improve = false;
    for(int i = 1; i <= instance.getNbJob(); i++) {

        transpose(sol, i);
        newCost = instance.computePartialWCTN(sol, i);

        if(bestImp) {
            if(i == 1 || newCost > bestNewCost) {
                bestNewCost = newCost;
                bestPos = i;
            }
            transpose(sol, i); // move is canceled
        } else {
            if(newCost > cost) {
                // first improvement, the move is applied directly
                instance.computePartialWCT(sol, i);
                improve = true;
                cost = newCost;
            }
        }
    }

    if(bestImp && bestNewCost > bestImp) {
        improve = true;
        transpose(sol, bestPos);
        cost = instance.computePartialWCT(sol, bestPos);
    }

    return improve;
}

/* Improve the solution */
void iterativeImprovement(PfspInstance& instance, vector< int > & sol, int& cost) {

  bool improvement = true;

  int bestCost; // best cost so far
  int aMin, bMin; // changes in the solution

  // init
  randomPermutation(instance.getNbJob(), sol);
  bestCost = instance.computePartialWCT(sol, 0);

  // step
  while(improvement) {

    // pivoting rule



    improvement = false;

  }

  // terminates

}

/* display the solution */
void displaySolution(vector<int>& sol) {
  for(int i = 1; i < sol.size(); i++) {
    cout << sol.at(i) << " ";
  }
  cout << endl;
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

  cout << "test : " << instance.computePartialWCT(solution, 1) << endl;
  // cout << "test2 : " << instance.computePartialWCT(solution, 1) << endl;
  cout << "test3 : " << instance.computePartialWCTN(solution, 3) << endl;


  //transpose(solution, 2);
  //displaySolution(solution);

  return 0;
}
