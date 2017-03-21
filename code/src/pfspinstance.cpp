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
#include <fstream>
#include <cmath>
#include <cstring>
#include "pfspinstance.h"


using namespace std;

PfspInstance::PfspInstance()
{
}


PfspInstance::~PfspInstance()
{
}

int PfspInstance::getNbJob()
{
    return nbJob;
}

int PfspInstance::getNbMac()
{
    return nbMac;
}

long int PfspInstance::getPriority(int i) {
    return priority.at(i);
}



/* Allow the memory for the processing times matrix : */
void PfspInstance::allowMatrixMemory(int nbJ, int nbM)
{
    processingTimesMatrix.resize(nbJ+1);          // 1st dimension

    for (int cpt = 0; cpt < nbJ+1; ++cpt)
    processingTimesMatrix[cpt].resize(nbM+1); // 2nd dimension

    dueDates.resize(nbJ+1);
    priority.resize(nbJ+1);

    completionTimesMatrix.resize(nbM+1);
    for(int cpt = 0; cpt < nbM+1; cpt++) {
        completionTimesMatrix.at(cpt).resize(nbJ+1);
    }
    completionTimesMatrix.at(0).at(1) = 0;
    completionTimesMatrix.at(1).at(0) = 0;

    partialCost.resize(nbJ+1);
    partialCost.at(0) = 0;

    previousJobEndTime.resize(nbM+1);
    previousJobEndTime.at(0) = 0;
}


long int PfspInstance::getTime(int job, int machine)
{
    if (job == 0)
    return 0;
    else
    {
        if ((job < 1) || (job > nbJob) || (machine < 1) || (machine > nbMac))
        cout    << "ERROR. file:pfspInstance.cpp, method:getTime. Out of bound. job=" << job
        << ", machine=" << machine << std::endl;

        return processingTimesMatrix[job][machine];
    }
}


/* Read the instance from file : */
bool PfspInstance::readDataFromFile(char * fileName)
{
    bool everythingOK = true;
    int j, m; // iterators
    long int readValue;
    string str;
    ifstream fileIn;
    char * aux2;
    char fileNameOK[100] = "";

    aux2 = (strrchr(fileName, '/'));

    if (aux2 == NULL)
    aux2 = fileName;
    else
    aux2 += 1;

    strcat(fileNameOK, aux2);

    cout << "name : " << fileNameOK << endl;
    cout << "file : " << fileName << endl;

    fileIn.open(fileName);

    if ( fileIn.is_open() ) {
        cout << "File " << fileName << " is now open, start to read..." << std::endl;

        fileIn >> nbJob;
        cout << "Number of jobs : " << nbJob << std::endl;
        fileIn >> nbMac;
        cout << "Number of machines : " << nbMac << std::endl;
        cout << "Allow memory for the matrix..." << std::endl;
        allowMatrixMemory(nbJob, nbMac);
        cout << "Memory allowed." << std::endl;
        cout << "Start to read matrix..." << std::endl;

        for (j = 1; j <= nbJob; ++j)
        {
            for (m = 1; m <= nbMac; ++m)
            {
                fileIn >> readValue; // The number of each machine, not important !
                fileIn >> readValue; // Process Time

                processingTimesMatrix[j][m] = readValue;
            }
        }
        fileIn >> str; // this is not read

        for (j = 1; j <= nbJob; ++j)
        {
            fileIn >> readValue; // -1
            fileIn >> readValue;
            dueDates[j] = readValue;
            fileIn >> readValue; // -1
            fileIn >> readValue;
            priority[j] = readValue;
        }

        cout << "All is read from file." << std::endl;
        fileIn.close();
    }
    else
    {
        cout    << "ERROR. file:pfspInstance.cpp, method:readDataFromFile, "
        << "error while opening file " << fileName << std::endl;

        everythingOK = false;
    }

    return everythingOK;
}


/* Compute the weighted tardiness of a given solution */
long int PfspInstance::computeWCT(vector< int > & sol)
{
    int j, m;
    int jobNumber;
    long int wct;

    /* We need end times on previous machine : */
    vector< long int > previousMachineEndTime ( nbJob + 1 );
    /* And the end time of the previous job, on the same machine : */
    long int previousJobEndTime;

    /* 1st machine : */
    previousMachineEndTime[0] = 0;
    for ( j = 1; j <= nbJob; ++j )
    {
        jobNumber = sol[j];
        previousMachineEndTime[j] = previousMachineEndTime[j-1] + processingTimesMatrix[jobNumber][1];
    }

    /* others machines : */
    for ( m = 2; m <= nbMac; ++m )
    {
        previousMachineEndTime[1] +=
        processingTimesMatrix[sol[1]][m];
        previousJobEndTime = previousMachineEndTime[1];

        for ( j = 2; j <= nbJob; ++j )
        {
            jobNumber = sol[j];

            if ( previousMachineEndTime[j] > previousJobEndTime )
            {
                previousMachineEndTime[j] = previousMachineEndTime[j] + processingTimesMatrix[jobNumber][m];
                previousJobEndTime = previousMachineEndTime[j];
            }
            else
            {
                previousJobEndTime += processingTimesMatrix[jobNumber][m];
                previousMachineEndTime[j] = previousJobEndTime;
            }
        }
    }

    wct = 0;
    for ( j = 1; j<= nbJob; ++j )
    wct += previousMachineEndTime[j] * priority[sol[j]];

    return wct;
}

long int PfspInstance::computePartialWCT(vector<int>& sol, int i) {

    return computePartialWCT(sol, i, nbJob);

}

long int PfspInstance::computePartialWCTN(vector<int>& sol, int i) {

    return computePartialWCTN(sol, i, nbJob);

}

/* compute the weighted completion time, starting from i to end (there are no changes before)*/
long int PfspInstance::computePartialWCT(vector<int>& sol, int i, int end) {

    int jobNumber;

    // first job
    if(i == 1) {
        jobNumber = sol.at(1);
        for(int m = 1; m <= nbMac; m++) {
            completionTimesMatrix.at(m).at(1) = completionTimesMatrix.at(m-1).at(1)+processingTimesMatrix.at(jobNumber).at(m);
        }
        partialCost.at(1) = priority.at(jobNumber)*completionTimesMatrix.at(nbMac).at(1);
    }

    // other jobs
    for(int j = max(i, 2); j <= end; j++) {
        jobNumber = sol.at(j);
        // first machine
        completionTimesMatrix.at(1).at(j) = completionTimesMatrix[1][j-1] + processingTimesMatrix[jobNumber][1];
        // other machines
        for(int m = 2; m <= nbMac; m++) {

            if(completionTimesMatrix[m-1][j] > completionTimesMatrix[m][j-1]) {
                completionTimesMatrix[m][j] = completionTimesMatrix[m-1][j] + processingTimesMatrix[jobNumber][m];
            } else {
                completionTimesMatrix[m][j] = completionTimesMatrix[m][j-1] + processingTimesMatrix[jobNumber][m];
            }

        }
        partialCost.at(j) = partialCost.at(j-1) + priority.at(jobNumber)*completionTimesMatrix.at(nbMac).at(j);
    }

    return partialCost.at(end);

}

/* compute the weighted completion time, starting from i to end whithout changing the completion times matrix*/
long int PfspInstance::computePartialWCTN(vector<int>& sol, int i, int end) {

    int jobNumber;
    long int previousMachineEndTime = 0;
    long int  res = partialCost.at(i-1);

    // first job
    if(i == 1) {
        jobNumber = sol.at(1);
        for(int m = 1; m <= nbMac; m++) {
            previousJobEndTime.at(m) = previousJobEndTime.at(m-1) + processingTimesMatrix.at(jobNumber).at(m);
        }
        res += previousJobEndTime.at(nbMac)*priority.at(jobNumber);
    }

    // other jobs
    for(int j = max(i, 2); j <= end; j++) {
        jobNumber = sol.at(j);

        if(j == i) {
            previousJobEndTime.at(1) = completionTimesMatrix.at(1).at(j-1);
        }

        // first machine
        previousMachineEndTime = previousJobEndTime.at(1) + processingTimesMatrix.at(jobNumber).at(1);
        previousJobEndTime.at(1) = previousMachineEndTime;

        // other machines
        for(int m = 2; m <= nbMac; m++) {

            if(j == i) {
                previousJobEndTime.at(m) = completionTimesMatrix.at(m).at(j-1);
            }

            if(previousMachineEndTime > previousJobEndTime.at(m)) {
                previousMachineEndTime += processingTimesMatrix.at(jobNumber).at(m);
            } else {
                previousMachineEndTime = previousJobEndTime.at(m)+processingTimesMatrix.at(jobNumber).at(m);
            }
            previousJobEndTime.at(m) = previousMachineEndTime;
        }
        res += previousJobEndTime.at(nbMac)*priority.at(jobNumber);
    }
    return res;

}
