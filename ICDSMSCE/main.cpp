/**
 * @file main.cpp
 * @date December 8, 2005
 * @brief This file defines the main function of Cayuga.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../ICDSMSCE/CayugaSystem.h"
using namespace Cayuga;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void insertUDFs();

/**
* @brief The main function of this program
* @param argc The number of arguments
* @param argv The sequence of argument string values
* @note argv[1] should contain the file name of the config XML file.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date December 8, 2005
* @version 1.0
*/
int main(int argc, char* argv[]) {
	try {
		if (argc < 2) {
			cerr<<"specify config XML file."<<endl;
			exit(-1);
		}

		insertUDFs();

		CayugaSystem* pCayuga = CayugaSystem::getInstance();
		pCayuga->init(argv[1]);
		
		pCayuga->startQP();
		getchar();


	}
	catch ( ... ) {
		cerr<<"uncaught exception occurred in main function."<<endl;
		return -1;
	}
}
