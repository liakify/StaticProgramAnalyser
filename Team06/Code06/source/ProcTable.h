#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"
#include "Simple.h"

using std::unordered_map;
using std::out_of_range;
using SIMPLE::Procedure;

namespace PKB {

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