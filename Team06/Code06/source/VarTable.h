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

    /** 
     *  The VarTable class stores all variable names extracted from the SIMPLE source code
     *  and tags them with an ID. 
     */
    class VarTable {
    
        unordered_map<VarName, VarId> nameIdTable;
        unordered_map<VarId, VarName> idNameTable;
        VarId varIdGenerator;
    
    public:
        
        VarTable();

        /**
         *  If varName is not in the VarTable, varName is inserted into the VarTable. 
         *  Else, the table remains unchanged.
         *  Returns the ID of the variable in the VarTable.
         */
        VarId insertVar(VarName varName);

        /**
         *  Returns the name of a variable at the given ID in the VarTable.
         *  Throws an exception if the id is not found in the table.
         */
        VarName get(VarId varId);

        /**
         *  Returns the ID of varName in the VarTable.If varName is not found, -1 is returned.
         */
        VarId getVarId(VarName varName);

        /**
         *  Returns all variables in the VarTable
         */
        std::unordered_set<VarName> getAllVars();
        
        /**
         *  Returns the number of variables in the VarTable
         */
        int size();

    };
}