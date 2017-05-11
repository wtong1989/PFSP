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

#include "localsearch.h"
#include "stochasticls.h"
#include "construction.h"

using namespace std;

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
  srand ( 12345 );

  /* Create instance object */
  PfspInstance instance;

  /* Read data from file */
  if (! instance.readDataFromFile(argv[1]) )
    return 1;

  vector< int > solution ( instance.getNbJob()+ 1 );

  grasp(instance, solution, totalWeightedTardiness, 0.4, 30000);
  // simulatedAnnealing(instance, solution, totalWeightedTardiness, 1000, 0.998, 100, 30000);

  // reactiveGrasp(instance, solution, totalWeightedTardiness, 10, 75, 10000);
  // simulatedAnnealing(instance, solution, totalWeightedTardiness, 700, 0.80, 100, 30000);

  cout << "best: " << totalWeightedTardiness << endl;

  cout << "recompute: " << instance.computeWCT(solution) << endl;

  return 0;
}

void test(int argc, char* argv[]) {

    int i;
    long int totalWeightedTardiness;


    if (argc == 1)
    {
      cout << "Usage: ./flowshopWCT <instance_file>" << endl;
      // return 0;
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
    // if (! instance.readDataFromFile(argv[1]) )
    //   return 1;

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

}
