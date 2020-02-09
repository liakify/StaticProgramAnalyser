#include "VarTable.h"

namespace PKB {

  VarTable::VarTable() {
    varIdGenerator = 0;
  }

  // If varName is not in the VarTable, varName is inserted into the VarTable. 
  // Else, the table remains unchanged.
  // Returns the ID of the variable in the VarTable.
  VarId VarTable::insertVar(VarName varName) {
    if (nameIdTable.try_emplace(varName, varIdGenerator).second) {
      VarId thisId = varIdGenerator++;
      idNameTable.insert(make_pair(thisId, varName));
      return thisId;
    }
    else {
      return getVarId(varName);
    }
  }

  // Returns the name of a variable at the given ID in the VarTable.
      // Throws an exception if the id is not found in the table.
  VarName VarTable::get(VarId varId) {
    return idNameTable.at(varId); // throws out_of_range exception
  }

  // Returns the ID of varName in the VarTable.If varName is not found, -1 is returned.
  VarId VarTable::getVarId(VarName varName) {
    try {
      return nameIdTable.at(varName);
    }
    catch (const out_of_range&) {
      return -1;
    }
  }

  // Returns the number of variables in the VarTable.
  int VarTable::size() {
    return nameIdTable.size();
  }
}