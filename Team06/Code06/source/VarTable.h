#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"

using namespace std;

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
    int size();

  };
}