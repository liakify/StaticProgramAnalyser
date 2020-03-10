#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"

using std::out_of_range;
using std::unordered_map;

namespace PKB {

    /**
     *  The ConstTable class stores all constants extracted from the SIMPLE source code
     *  and tags them with an ID.
     */
    class ConstTable {

        unordered_map<ConstValue, ConstId> valueIdTable;
        unordered_map<ConstId, ConstValue> idValueTable;
        ConstId ConstIdGenerator;

     public:

        ConstTable();

        /**
         *  If ConstValue is not in the ConstTable, ConstValue is inserted into the ConstTable.
         *  Else, the table remains unchanged.
         *  Returns the ID of the constant in the ConstTable.
         */
        ConstId insertConst(ConstValue ConstValue);

        /**
         *  Returns the name of a constant at the given ID in the ConstTable.
         *  Throws an exception if the id is not found in the table.
         */
        ConstValue get(ConstId ConstId);

        /**
         *  Returns the ID of ConstValue in the ConstTable.If ConstValue is not found, -1 is returned.
         */
        ConstId getConstId(ConstValue ConstValue);

        /**
         *  Returns the number of constants in the ConstTable
         */
        int size();

    };
}