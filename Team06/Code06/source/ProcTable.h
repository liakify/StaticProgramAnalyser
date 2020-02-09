#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"

using namespace std;

namespace PKB {

  class Procedure {

  public:
    ProcName procName;
    Procedure(ProcName procName);
    bool operator== (const Procedure& p);
  };

  class ProcTable {

    unordered_map<ProcName, ProcId> nameIdTable;
    unordered_map<ProcId, Procedure> idProcTable;
    ProcId procIdGenerator;

  public:
    
    ProcTable();
    ProcId insertProc(Procedure proc);
    Procedure get(ProcId procId);
    ProcId getProcId(ProcName procName);
    int size();

  };
}