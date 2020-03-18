#include <stdexcept>

#include "Simple.h"

namespace SIMPLE {
    Expression::Expression(Expression left, Expression right, char op)
        : left(left), right(right), op(op) {
        this->str = getStr();
        this->varSet = std::unordered_set<VarId>(left.varSet);
        this->varSet.insert(right.varSet.begin(), right.varSet.end());
        this->constSet = std::unordered_set<ConstId>(left.constSet);
        this->constSet.insert(right.constSet.begin(), right.constSet.end());
        this->patterns = std::unordered_set<Pattern>(left.patterns);
        this->patterns.insert(right.patterns.begin(), right.patterns.end());
        this->patterns.insert("_" + this->str + "_");
    }

    Expression::Expression(std::string name, int id, ExprType type)
        : left(*this), right(*this), op('\0'), str(name) {
        if (type == ExprType::VAR) {
            this->varSet.insert(id);
            this->patterns.insert("_" + this->str + "_");
        } else if (type == ExprType::CONST) {
            this->constSet.insert(id);
            this->patterns.insert("_" + this->str + "_");
        } else {
            throw std::invalid_argument("ExprType is not ExprType::VAR or ExprType::CONST");
        }
    }

    std::unordered_set<VarId> Expression::getVarIds() {
        return this->varSet;
    }

    std::unordered_set<ConstId> Expression::getConstIds() {
        return this->constSet;
    }

    std::unordered_set<Pattern> Expression::getPatterns() {
        return patterns;
    }

    char Expression::getOp() {
        return this->op;
    }

    std::string Expression::getStr() {
        if (this->str != "") {
            return this->str;
        } else {
            return "(" + this->left.getStr() + this->op + this->right.getStr() + ")";
        }
    }

    CondExpr::CondExpr(CondExpr left, CondExpr right)
        : leftCond(&left), rightCond(&right), leftFactor(nullptr), rightFactor(nullptr) {
        this->varSet = std::unordered_set<VarId>(left.varSet);
        this->varSet.insert(right.varSet.begin(), right.varSet.end());
        this->constSet = std::unordered_set<ConstId>(left.constSet);
        this->constSet.insert(right.constSet.begin(), right.constSet.end());
    }

    CondExpr::CondExpr(Expression left, Expression right)
        : leftCond(nullptr), rightCond(nullptr), leftFactor(&left), rightFactor(&right) {
        this->varSet = std::unordered_set<VarId>(left.getVarIds());
        std::unordered_set<VarId> temp = std::unordered_set<VarId>(right.getVarIds());
        this->varSet.insert(temp.begin(), temp.end());
        this->constSet = std::unordered_set<ConstId>(left.getConstIds());
        std::unordered_set<ConstId> temp2 = std::unordered_set<ConstId>(right.getConstIds());
        this->constSet.insert(temp2.begin(), temp2.end());
    }

    std::unordered_set<VarId> CondExpr::getVarIds() {
        return this->varSet;
    }

    std::unordered_set<ConstId> CondExpr::getConstIds() {
        return this->constSet;
    }

    Statement::Statement() {}

    StmtType Statement::getType() {
        return this->stmtType;
    }

    void Statement::setContainerId(StmtListId sid) {
        this->containerId = sid;
    }

    StmtListId Statement::getContainerId() {
        return this->containerId;
    }

    PrintStmt::PrintStmt(VarId var)
        : var(var) {
        this->stmtType = StmtType::PRINT;
    }

    VarId PrintStmt::getVar() {
        return this->var;
    }

    ReadStmt::ReadStmt(VarId var)
        : var(var) {
        this->stmtType = StmtType::READ;
    }

    VarId ReadStmt::getVar() {
        return this->var;
    }

    IfStmt::IfStmt(CondExpr cond, StmtListId thenStmtLst, StmtListId elseStmtLst)
        : condExpr(cond), thenStmtLst(thenStmtLst), elseStmtLst(elseStmtLst) {
        this->stmtType = StmtType::IF;
    }

    CondExpr IfStmt::getCondExpr() {
        return this->condExpr;
    }

    StmtListId IfStmt::getThenStmtLstId() {
        return this->thenStmtLst;
    }

    StmtListId IfStmt::getElseStmtLstId() {
        return this->elseStmtLst;
    }

    WhileStmt::WhileStmt(CondExpr cond, StmtListId stmtLst)
        : condExpr(cond), stmtLst(stmtLst) {
        this->stmtType = StmtType::WHILE;
    }

    CondExpr WhileStmt::getCondExpr() {
        return this->condExpr;
    }

    StmtListId WhileStmt::getStmtLstId() {
        return this->stmtLst;
    }

    CallStmt::CallStmt(ProcName procName)
        : procName(procName) {
        this->stmtType = StmtType::CALL;
    }

    ProcName CallStmt::getProc() {
        return this->procName;
    }

    AssignStmt::AssignStmt(VarId var, Expression expr)
        : var(var), expr(expr) {
        this->stmtType = StmtType::ASSIGN;
    }

    VarId AssignStmt::getVar() {
        return this->var;
    }

    Expression AssignStmt::getExpr() {
        return this->expr;
    }

    StatementList::StatementList(std::vector<StmtId>& statements)
        : statements(statements), first(statements.front()), last(statements.back()) {
        this->allEnds = std::unordered_set<StmtId>();
    }

    bool StatementList::operator== (const StatementList& other) {
        return this->statements == other.statements;
    }

    std::vector<StmtId> StatementList::getStmtIds() {
        return this->statements;
    }

    void StatementList::setLast(StmtId last) {
        this->last = last;
    }

    void StatementList::addEnd(StmtId end) {
        this->allEnds.insert(end);
    }

    StmtId StatementList::getFirst() {
        return this->first;
    }

    StmtId StatementList::getLast() {
        return this->last;
    }

    std::unordered_set<StmtId>& StatementList::getAllEnds() {
        return this->allEnds;
    }

    Procedure::Procedure(ProcName procName, StmtListId stmtLstId)
        : procName(procName), stmtLstId(stmtLstId) {
    }

    bool Procedure::operator== (const Procedure& p) {
        return procName == p.procName;
    }

    ProcName Procedure::getName() {
        return this->procName;
    }

    StmtListId Procedure::getStmtLstId() {
        return this->stmtLstId;
    }
}
