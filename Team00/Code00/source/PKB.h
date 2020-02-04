#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
typedef short PROC;
typedef string VarName;
typedef int VarId;


class TNode;

// class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class VarTable {
	static unordered_map<VarName, VarId> nameIdTable;
	static unordered_map<VarId, VarName> idNameTable;
	static VarId varIdGenerator;
public:

	// If varName is not in the VarTable, varName is inserted into the VarTable. Else, the table remains unchanged.
	// Returns the ID of the variable in the VarTable.
	VarId insertVar(VarName varName) {
		if (nameIdTable.try_emplace(varName, varIdGenerator).second) {
			VarId thisId = varIdGenerator++;
			idNameTable.insert(make_pair(thisId, varName));
			return thisId;
		}
		else {
			return getVarId(varName);
		}
	}

	// Returns the name of a variable at the given ID in the VarTable.Throws an exception if the id is not found in the table.
	VarName get(VarId varId) {
		return idNameTable.at(varId); // throws out_of_range exception
	}

	// Returns the ID of varName in the VarTable.If varName is not found, -1 is returned.
	VarId getVarId(VarName varName) {
		try {
			return nameIdTable.at(varName);
		}
		catch(const out_of_range &e) {
			return -1;
		}
	}

	// Returns the number of variables in the VarTable.
	int size() {
		return nameIdTable.size();
	}
};

class ProcTable {

};

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);
};