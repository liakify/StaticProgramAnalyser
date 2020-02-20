#include "ConstTable.h"

namespace PKB {

  ConstTable::ConstTable() {
    ConstIdGenerator = 1;
  }

  ConstId ConstTable::insertConst(ConstValue ConstValue) {
    if (valueIdTable.try_emplace(ConstValue, ConstIdGenerator).second) {
      ConstId thisId = ConstIdGenerator++;
      idValueTable.insert(make_pair(thisId, ConstValue));
      return thisId;
    }
    else {
      return getConstId(ConstValue);
    }
  }

  ConstValue ConstTable::get(ConstId ConstId) {
    return idValueTable.at(ConstId); // throws out_of_range exception
  }


  ConstId ConstTable::getConstId(ConstValue ConstValue) {
    try {
      return valueIdTable.at(ConstValue);
    }
    catch (const out_of_range&) {
      return -1;
    }
  }

  int ConstTable::size() {
    return valueIdTable.size();
  }
}