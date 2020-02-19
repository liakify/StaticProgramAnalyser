#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Types.h"

using std::out_of_range;
using std::unordered_map;

namespace PKB {

  class VarTable {
  
    unordered_map<VarName, VarId> nameIdTable;
    unordered_map<VarId, VarName> idNameTable;
    VarId varIdGenerator;
  
  public:
    
    VarTable();
    VarId insertVar(VarName varName);
    VarName get(VarId varId);
    VarId getVarId(VarName varName);
    std::unordered_set<VarName> getAllVars();
    int size();

  };
}