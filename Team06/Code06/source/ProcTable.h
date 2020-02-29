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

  /**
   *  The ProcTable class stores all Procedure names extracted from the SIMPLE source code
   *  and tags them with an ID.
   */
  class ProcTable {

    unordered_map<ProcName, ProcId> nameIdTable;
    unordered_map<ProcId, Procedure> idProcTable;
    ProcId procIdGenerator;

  public:
    
    ProcTable();

    /**
     *  If proc is not in the ProcTable, proc is inserted into the ProcTable. 
     *  Else, an exception is thrown.
     *  
     *  @param      Procedure proc to insert
     *  @return     ID of the procedure in the ProcTable. 
     *  @throws     std::invalid_argument if procName already exists
     */
    ProcId insertProc(Procedure proc);

    /**
     *  Returns the procedure object at the given ID in the ProcTable. 
     *  Throws an exception if the ID is not found in the table.
     */
    Procedure get(ProcId procId);
    
    /**
     * Returns the ID of proc in the ProcTable. If proc is not found, -1 is returned.
     */
    ProcId getProcId(ProcName procName);

    /** 
     *  Returns the number of procedures in the ProcTable.
     */
    int size();

  };
}