#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "StmtListTable.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "FollowsKB.h"
#include "ParentKB.h"
#include "UsesKB.h"
#include "ModifiesKB.h"
#include "PatternKB.h"

namespace PKB {
  class PKB {
  public:
    VarTable varTable;
    ProcTable procTable;
    StmtListTable stmtListTable;
    StmtTable stmtTable;
    FollowsKB followsKB;
    ParentKB parentKB;
    UsesKB usesKB;
    ModifiesKB modifiesKB;
    PatternKB patternKB;
  };
}
