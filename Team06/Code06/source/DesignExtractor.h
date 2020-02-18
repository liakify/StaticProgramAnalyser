#pragma once

#include <unordered_set>

#include "Types.h"
#include "PKB.h"

class DesignExtractor {
public:
	DesignExtractor(PKB::PKB& pkb);

	void run();

private:
	PKB::PKB pkb;

	void populateFollowStar();

	void populateParentStar();
};