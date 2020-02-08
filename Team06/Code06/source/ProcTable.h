#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"

using namespace std;

class Procedure {
public:
  ProcName procName;
  Procedure(ProcName procName) {
    this->procName = procName;
  }

  bool operator== (const Procedure& p) {
    return procName == p.procName;
  }
};

class ProcTable {

  unordered_map<ProcName, ProcId> nameIdTable;
  unordered_map<ProcId, Procedure> idProcTable;
  ProcId procIdGenerator;

public:

  ProcTable() {
    procIdGenerator = 0;
  }

  // If proc is not in the ProcTable, proc is inserted into the ProcTable. 
  // Else, the table remains unchanged.
  // Returns the ID of the procedure in the ProcTable.
  ProcId insertProc(Procedure proc) {
    ProcName procName = proc.procName;
    if (nameIdTable.try_emplace(procName, procIdGenerator).second) {
      ProcId thisId = procIdGenerator;
      procIdGenerator++;
      idProcTable.insert(make_pair(thisId, proc));
      return thisId;
    }
    else {
      return getProcId(procName);
    }
  }

  // Returns the procedure object at the given ID in the ProcTable. 
  // Throws an exception if the ID is not found in the table.
  Procedure get(ProcId procId) {
    return idProcTable.at(procId); // throws out_of_range exception
  }

  // Returns the ID of proc in the ProcTable. If proc is not found, -1 is returned.
  ProcId getProcId(ProcName procName) {
    try {
      return nameIdTable.at(procName);
    }
    catch (const out_of_range &) {
      return -1;
    }
  }

  // Returns the number of procedures in the ProcTable.
  int size() {
    return nameIdTable.size();
  }
};