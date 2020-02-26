#pragma once

#include "Types.h"
#include "PQLTypes.h"

namespace SPA {
	namespace TypeUtils {

		bool isStmtTypeDesignEntity(StmtType stmtType, DesignEntity designEntity) {
			switch (designEntity) {
			case DesignEntity::STATEMENT:
				return true;
				break;
			case DesignEntity::READ:
				return stmtType == StmtType::READ;
				break;
			case DesignEntity::PRINT:
				return stmtType == StmtType::PRINT;
				break;
			case DesignEntity::CALL:
				return stmtType == StmtType::CALL;
				break;
			case DesignEntity::WHILE:
				return stmtType == StmtType::WHILE;
				break;
			case DesignEntity::IF:
				return stmtType == StmtType::IF;
				break;
			case DesignEntity::ASSIGN:
				return stmtType == StmtType::ASSIGN;
				break;
			default:
				return false;
			}
		}

	}
}