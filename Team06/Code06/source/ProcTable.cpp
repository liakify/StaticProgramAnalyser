#include "ProcTable.h"

namespace PKB {

  ProcTable::ProcTable() {
    procIdGenerator = 0;
  }

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

  Procedure ProcTable::get(ProcId procId) {
    return idProcTable.at(procId); // throws out_of_range exception
  }

  ProcId ProcTable::getProcId(ProcName procName) {
    try {
      return nameIdTable.at(procName);
    }
    catch (const out_of_range&) {
      return -1;
    }
  }

  int ProcTable::size() {
    return nameIdTable.size();
  }
}