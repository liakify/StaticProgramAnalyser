#include <vector>

#include "Parser.h"

using std::invalid_argument;
using std::regex;
using std::shared_ptr;
using std::string;

namespace FrontEnd {
    PKB::PKB Parser::parseSimple(string src) {
        this->pkb = PKB::PKB();
        this->src = src;
        this->pos = 0;
        program();
        if (this->pos != src.length()) {
            throw invalid_argument("Syntax error in SIMPLE source.\n");
        }
        return this->pkb;
    }

    string Parser::parseExpression(string exp) {
        this->pkb = PKB::PKB();
        this->src = exp;
        this->pos = 0;
        string result = expr().getStr();
        if (this->pos != this->src.length()) {
            throw invalid_argument("Syntax error in expression.\n");
        }
        return result;
    }

    string Parser::consume(regex rgx) {
        std::smatch match;
        string str = src.substr(this->pos);
        if (!regex_search(str, match, rgx, std::regex_constants::match_continuous)) {
            throw invalid_argument("Syntax error in SIMPLE source.\n");
        }
        this->pos += match.length();
        return match.str();
    }

    string Parser::name() {
        return consume(regex("[A-Z|a-z][A-Z|a-z|0-9]*"));
    }

    string Parser::integer() {
        string result = consume(regex("[0-9]+"));
        if (result[0] == '0' && result.length() > 1) {
            throw invalid_argument("Syntax error in SIMPLE source.\n");
        }
        return result;
    }

    void Parser::program() {
        size_t currentPos = this->pos;
        procedure();
        while (currentPos < this->pos) {
            try {
                currentPos = this->pos;
                procedure();
            }
            catch (invalid_argument & e) {
                if (strcmp(e.what(), "Duplicate procName detected") == 0) {
                    throw e;
                }
            }
        }
    }

    ProcId Parser::procedure() {
        consume(regex("[\\s]*procedure[\\s]+"));
        ProcName pn = proc_name();
        consume(regex("[\\s]*[{][\\s]*"));
        StmtListId sl = stmtLst();
        consume(regex("[\\s]*[}][\\s]*"));
        Procedure p = Procedure(pn, sl);
        return pkb.procTable.insertProc(p);
    }

    StmtListId Parser::stmtLst() {
        std::vector<StmtId> statements;
        statements.push_back(stmt());
        while (true) {
            try {
                statements.push_back(stmt());
            } catch (const invalid_argument&) {
                break;
            }
        }
        StatementList sl = StatementList(statements);
        return pkb.stmtListTable.insertStmtLst(sl);
    }

    StmtId Parser::stmt() {
        size_t currentPos = this->pos;
        try {
            return pkb.stmtTable.insertStmt(read_stmt());
        } catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            return pkb.stmtTable.insertStmt(print_stmt());
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            return pkb.stmtTable.insertStmt(call_stmt());
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            StmtId stmtId = pkb.stmtTable.reserveId();
            pkb.stmtTable.insertStmtAtId(while_stmt(), stmtId);
            return stmtId;
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
            pkb.stmtTable.unreserveId();
        }
        try {
            StmtId stmtId = pkb.stmtTable.reserveId();
            pkb.stmtTable.insertStmtAtId(if_stmt(), stmtId);
            return stmtId;
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
            pkb.stmtTable.unreserveId();
        }
        return pkb.stmtTable.insertStmt(assign_stmt());
    }

    shared_ptr<ReadStmt> Parser::read_stmt() {
        consume(regex("[\\s]*read[\\s]+"));
        VarId v = var_name();
        consume(regex("[\\s]*[;][\\s]*"));
        return shared_ptr<ReadStmt>(new ReadStmt(v));
    }

    shared_ptr<PrintStmt> Parser::print_stmt() {
        consume(regex("[\\s]*print[\\s]+"));
        VarId v = var_name();
        consume(regex("[\\s]*[;][\\s]*"));
        return shared_ptr<PrintStmt>(new PrintStmt(v));
    }

    shared_ptr<CallStmt> Parser::call_stmt() {
        consume(regex("[\\s]*call[\\s]+"));
        ProcName p = proc_name();
        consume(regex("[\\s]*[;][\\s]*"));
        return shared_ptr<CallStmt>(new CallStmt(p));
    }

    shared_ptr<WhileStmt> Parser::while_stmt() {
        consume(regex("[\\s]*while[\\s]*[(][\\s]*"));
        CondExpr cond = cond_expr();
        consume(regex("[\\s]*[)][\\s]*[{][\\s]*"));
        StmtListId sl = stmtLst();
        consume(regex("[\\s]*[}][\\s]*"));
        return shared_ptr<WhileStmt>(new WhileStmt(cond, sl));
    }

    shared_ptr<IfStmt> Parser::if_stmt() {
        consume(regex("[\\s]*if[\\s]*[(][\\s]*"));
        CondExpr cond = cond_expr();
        consume(regex("[\\s]*[)][\\s]*then[\\s]*[{][\\s]*"));
        StmtListId thenStmtLst = stmtLst();
        consume(regex("[\\s]*[}][\\s]*else[\\s]*[{][\\s]*"));
        StmtListId elseStmtLst = stmtLst();
        consume(regex("[\\s]*[}][\\s]*"));
        return shared_ptr<IfStmt>(new IfStmt(cond, thenStmtLst, elseStmtLst));
    }

    shared_ptr<AssignStmt> Parser::assign_stmt() {
        VarId v = var_name();
        consume(regex("[\\s]*[=][\\s]*"));
        Expression exp = expr();
        consume(regex("[\\s]*[;][\\s]*"));
        return shared_ptr<AssignStmt>(new AssignStmt(v, exp));
    }

    CondExpr Parser::cond_expr() {
        size_t currentPos = this->pos;
        try {
            return rel_expr();
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            consume(regex("[\\s]*[!][\\s]*[(][\\s]*"));
            CondExpr negated = cond_expr();
            consume(regex("[\\s]*[)][\\s]*"));
            return negated;
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            consume(regex("[\\s]*[(][\\s]*"));
            CondExpr left = cond_expr();
            consume(regex("[\\s]*[)][\\s]*(&&)[\\s]*[(][\\s]*"));
            CondExpr right = cond_expr();
            consume(regex("[\\s]*[)][\\s]*"));
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        consume(regex("[\\s]*[(][\\s]*"));
        CondExpr left = cond_expr();
        consume(regex("[\\s]*[)][\\s]*(\\|\\|)[\\s]*[(][\\s]*"));
        CondExpr right = cond_expr();
        consume(regex("[\\s]*[)][\\s]*"));
        return CondExpr(left, right);
    }

    CondExpr Parser::rel_expr() {
        size_t currentPos = this->pos;
        try {
            Expression left = expr();
            consume(regex("[\\s]*(>)[\\s]*"));
            Expression right = expr();
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            Expression left = expr();
            consume(regex("[\\s]*(>=)[\\s]*"));
            Expression right = expr();
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            Expression left = expr();
            consume(regex("[\\s]*(<)[\\s]*"));
            Expression right = expr();
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            Expression left = expr();
            consume(regex("[\\s]*(<=)[\\s]*"));
            Expression right = expr();
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            Expression left = expr();
            consume(regex("[\\s]*(==)[\\s]*"));
            Expression right = expr();
            return CondExpr(left, right);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        Expression left = expr();
        consume(regex("[\\s]*(!=)[\\s]*"));
        Expression right = expr();
        return CondExpr(left, right);
    }

    // -1 if op1 < op2, 0 if op1 == op2, 1 if op1 > op2
    int Parser::compare_op(char op1, char op2) {
        return opRank.at(op1) - opRank.at(op2);
    }

    void Parser::combine_op(std::stack<Expression>& operands, std::stack<char>& operators) {
        Expression right = operands.top();
        operands.pop();
        Expression left = operands.top();
        operands.pop();
        operands.push(Expression(left, right, operators.top()));
        operators.pop();
    }

    Expression Parser::expr() {
        char op;
        std::stack<Expression> operands;
        std::stack<char> operators;
        consume(regex("[\\s]*"));
        Expression token = factor();
        operands.push(token);
        while (true) {
            try {
                consume(regex("[\\s]*"));
                op = consume(regex("[\\+\\%\\*\\-\\/][\\s]*"))[0];
                while (!operators.empty() && compare_op(operators.top(), op) != -1) {
                    combine_op(operands, operators);
                }
                operators.push(op);
                operands.push(factor());
            }
            catch (const invalid_argument & e) {
                if (operands.size() - operators.size() != 1) {
                    throw e;
                }
                break;
            }
        }
        while (!operators.empty() && operands.size() > 1) {
            combine_op(operands, operators);
        }
        consume(regex("[\\s]*"));
        return operands.top();
    }

    Expression Parser::factor() {
        size_t currentPos = this->pos;
        try {
            VarId id = var_name();
            VarName name = pkb.varTable.get(id);
            return Expression(name, id, ExprType::VAR);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        try {
            ConstId id = const_value();
            ConstValue value = pkb.constTable.get(id);
            return Expression(value, id, ExprType::CONST);
        }
        catch (const invalid_argument&) {
            this->pos = currentPos;
        }
        consume(regex("[\\s]*[(][\\s]*"));
        Expression opr = expr();
        consume(regex("[\\s]*[)][\\s]*"));
        return opr;
    }

    VarId Parser::var_name() {
        return pkb.varTable.insertVar(name());
    }

    ProcName Parser::proc_name() {
        return name();
    }

    ConstId Parser::const_value() {
        return pkb.constTable.insertConst(integer());
    }
}