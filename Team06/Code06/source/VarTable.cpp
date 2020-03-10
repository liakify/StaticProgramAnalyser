#include "VarTable.h"

namespace PKB {

    VarTable::VarTable() {
        varIdGenerator = 1;
    }

    VarId VarTable::insertVar(VarName varName) {
        if (nameIdTable.try_emplace(varName, varIdGenerator).second) {
            VarId thisId = varIdGenerator++;
            idNameTable.insert(make_pair(thisId, varName));
            return thisId;
        } else {
            return getVarId(varName);
        }
    }

    VarName VarTable::get(VarId varId) {
        return idNameTable.at(varId);  // throws out_of_range exception
    }


    VarId VarTable::getVarId(VarName varName) {
        try {
            return nameIdTable.at(varName);
        }
        catch (const out_of_range&) {
            return -1;
        }
    }

    // Returns all variables in the VarTable.
    std::unordered_set<VarName> VarTable::getAllVars() {
            std::unordered_set<VarName> vars;
            for (std::pair<VarName, VarId> var : nameIdTable) {
                    vars.insert(var.first);
            }
            return vars;
    }

    // Returns the number of variables in the VarTable.
    int VarTable::size() {
        return nameIdTable.size();
    }
}