#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;

/* display a solution */
void displaySolution(vector<int>& sol) {
  for(int i = 1; i < sol.size(); i++) {
    cout << sol.at(i) << " ";
  }
  cout << endl;
}

/* generate a random position */
int generateRndPosition(int min, int max)
{
  return ( rand() % max + min );
}

double rndNumber() {
    return (double)rand() / (double)RAND_MAX;
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

/* setup the param data structure with the command line parameters (implementation exercise 2)*/
void extractParameters2(char* cmd[], int nb, parameters2& param) {

    param.algo = false;
    param.instanceName = -1;
    param.seed = 42;
    param.timeLimit = 42;

    int i = 1;

    while(i < nb) {

        if(string(cmd[i]) == "--instance") {
            i ++;
            param.instanceName = i;
        } else if(string(cmd[i]) == "--algo") {
            i ++;
            if(string(cmd[i]) == "sa") {
                param.algo = false;
            } else if(string(cmd[i]) == "grasp") {
                param.algo = true;
            }
        } else if(string(cmd[i]) == "--seed") {
            i ++;
            istringstream seedStream(cmd[i]);
            seedStream >> param.seed;
        } else if(string(cmd[i]) == "--time") {
            i ++;
            istringstream timeStream(cmd[i]);
            timeStream >> param.timeLimit;
        }

        i ++;
    }

    cout << "Parameters: " << endl;
    cout << "\tinstance: " << cmd[param.instanceName] << endl;
    cout << "\tseed: " << param.seed << endl;
    cout << "\talgorithme: " << (param.algo ? "grasp" : "simulated annealing") << endl;
    cout << "\ttime limit: " << param.timeLimit << endl;
}
