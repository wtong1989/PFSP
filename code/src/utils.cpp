#include "utils.h"
#include <iostream>
#include <cstdlib>

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
