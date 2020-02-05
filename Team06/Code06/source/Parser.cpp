#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"
#include "Parser.h"

void program(string::iterator it, string::iterator end);

int analyse(string& src) {
	program(src.begin(), src.end());
	return 0;
}

void program(string::iterator it, string::iterator end) {
	while (it != end) cout << *it++;
}
