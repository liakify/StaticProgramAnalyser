#pragma once

#include <unordered_set>

#include "Types.h"
#include "PKB.h"
#include "Simple.h"

#include <stdexcept>

using namespace SIMPLE;

namespace FrontEnd {
	/**
	* The DesignExtractor class is used for extracting secondary relationships
	* from the the parsed SIMPLE code, and populates the PKB accordingly.
	*/
	class DesignExtractor {
	public:
		/**
		* Updates the PKB with secondary relationships.
		* Relationships handled:
		*	Follows*
		*	Parent*
		*
		* @param	pkb			Reference to PKB to be updated.
		* @return	PKB::PKB	The updated PKB instance.
		*/
		PKB::PKB run(PKB::PKB& pkb);

	private:
		PKB::PKB pkb;

		// Functions to be called in run()
		void populateFollows();
		void populateFollowStar();
		void populateParent();
		void populateParentStar();
		void populateCallStar();
		void populateUses();
		void populateModifies();
		void populatePattern();

		// Helper functions
		void populateParentKB(StmtId stmtId, StmtListId stmtLstId);

		void populateStmtUses(StmtId id);
		std::unordered_set<VarId> getAllUses(StmtListId sid);
		void populateStmtUsesKB(StmtId stmtId, std::unordered_set<VarId>& varSet);
		void populateProcUsesKB(ProcId procId, std::unordered_set<VarId>& varSet);

		void populateStmtModifies(StmtId id);
		std::unordered_set<VarId> getAllModifies(StmtListId sid);
		void populateStmtModifiesKB(StmtId stmtId, std::unordered_set<VarId>& varSet);
		void populateProcModifiesKB(ProcId procId, std::unordered_set<VarId>& varSet);

		void populatePatternKB(StmtId stmtId, Expression exp);

		void dfsFromRoot(ProcId root, std::vector<ProcId>& visited);
		void dfsFromLeaf(ProcId leaf, std::vector<ProcId>& visited);
	};
}