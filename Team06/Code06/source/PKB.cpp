#include <string>
#include <vector>

#include "PKB.h"

namespace PKB {
    PKB::PKB()
        : nextKB(this) {
    }

    void PKB::clear() {
        nextKB.clear();
    }
}
