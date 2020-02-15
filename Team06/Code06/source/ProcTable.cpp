#include "ProcTable.h"

namespace PKB {

  ProcTable::ProcTable() {
    procIdGenerator = 0;
  }

  // If proc is not in the ProcTable, proc is inserted into the ProcTable. 
  // Else, the table remains unchanged.
  // Returns the ID of the procedure in the ProcTable.
  ProcId ProcTable::insertProc(Procedure proc) {
    ProcName procName = proc.getName();
    if (nameIdTable.try_emplace(procName, procIdGenerator).second) {
      ProcId thisId = procIdGenerator;
      procIdGenerator++;
      idProcTable.insert(std::make_pair(thisId, proc));
      return thisId;
    }
    else {
      return getProcId(procName);
    }
  }

  // Returns the procedure object at the given ID in the ProcTable. 
  // Throws an exception if the ID is not found in the table.
  Procedure ProcTable::get(ProcId procId) {
    return idProcTable.at(procId); // throws out_of_range exception
  }

  // Returns the ID of proc in the ProcTable. If proc is not found, -1 is returned.
  ProcId ProcTable::getProcId(ProcName procName) {
    try {
      return nameIdTable.at(procName);
    }
    catch (const out_of_range&) {
      return -1;
    }
  }

  // Returns the number of procedures in the ProcTable.
  int ProcTable::size() {
    return nameIdTable.size();
  }
}