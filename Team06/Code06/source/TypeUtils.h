#pragma once

#include "Types.h"
#include "PQLTypes.h"

namespace SPA {
    namespace TypeUtils {

        /**
        * Checks if a given StmtType corresponds to a given DesignEntity.
        *
        * @param    stmtType        The StmtType to check.
        * @param    designEntity    The DesignEntity to check.
        * @return   true if the StmtType corresponds to the given Designtity, false otherwise.
        */
        bool isStmtTypeDesignEntity(StmtType stmtType, DesignEntity designEntity);

        /**
        * Returns the StmtType that corresponds to a given DesignEntity.
        *
        * @param    designEntity    The DesignEntity to convert.
        * @return   The StmtType that corresponds to the given DesignEntity, or ERROR if the given DesignEntity is invalid.
        */
        StmtType getStmtTypeFromDesignEntity(DesignEntity designEntity);
    }
}