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

  /**
     *  The PKB class is the component in SPA that stores the data extracted by the 
     *  Front-End Parser. It has virtually no logical functionality less input and ouput.
     *  However, in the event that additional computation is needed by a query, the PKB
     *  calls upon the Design Extractor in the Front End.
     */
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
