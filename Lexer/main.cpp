/**************
Author: Garett Pascual-Folster
ID: 2001691416
Assignement 3 
***************/

#include <iostream>
#include <fstream>
#include "Lexer.h"

using namespace std;
int main() {
	// get input file
	ifstream *src = new ifstream("Source.txt", ifstream::in);
	if (!src->is_open())
		return -1;



	return 0;
}