#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "StmtListTable.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"

namespace PKB {
  class PKB {
  public:
    static VarTable varTable;
    static ProcTable procTable;
    static StmtListTable stmtListTable;
    static StmtTable stmtTable;
  };
}