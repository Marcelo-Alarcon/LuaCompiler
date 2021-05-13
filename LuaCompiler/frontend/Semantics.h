#ifndef SEMANTICS_H_
#define SEMANTICS_H_

#include <map>

#include "LuaBaseVisitor.h"
#include "antlr4-runtime.h"

#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/symtab/Predefined.h"
#include "intermediate/type/Typespec.h"
#include "SemanticErrorHandler.h"

namespace frontend {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

class Semantics : public LuaBaseVisitor
{
private:
    SymtabStack *symtabStack;
    SymtabEntry *programId;
    SemanticErrorHandler error;
    map<string, Typespec *> *typeTable;

    /**
     * Return the number of values in a datatype.
     * @param type the datatype.
     * @return the number of values.
     */
    int typeCount(Typespec *type);

    /**
     * Determine whether or not an expression is a variable only.
     * @param exprCtx the ExpressionContext.
     * @return true if it's an expression only, else false.
     */
    bool expressionIsVariable(LuaParser::ExpContext *exprCtx);

    /**
     * Perform semantic operations on procedure and function call arguments.
     * @param listCtx the ArgumentListContext.
     * @param parameters the vector of parameters to fill.
     */
    void checkCallArguments(LuaParser::ArgsContext *listCtx, vector<SymtabEntry *> *parms);


public:
    string programName;
    Semantics(string);

    /**
    * Get the symbol table entry of the program identifier.
    * @return the entry.
    */
    SymtabEntry *getProgramId() { return programId; }

    /**
     * Get the count of semantic errors.
     * @return the count.
     */
    int getErrorCount() const { return error.getCount(); }

    /**
     * Return the default value for a given datatype.
     * @param type the datatype.
     */
    static Object defaultValue(Typespec *type);

    Object visitChunk(LuaParser::ChunkContext *ctx) override;
    Object visitBlock(LuaParser::BlockContext *ctx) override;
    Object visitStat(LuaParser::StatContext *ctx) override;
	Object visitRetstat(LuaParser::RetstatContext *ctx) override;
	Object visitAssignStat(LuaParser::AssignStatContext *ctx) override;
	Object visitRepeatStat(LuaParser::RepeatStatContext *ctx) override;
	Object visitPrintStat(LuaParser::PrintStatContext *ctx) override;
	Object visitPrintArguments(LuaParser::PrintArgumentsContext *ctx) override;
	Object visitIfStat(LuaParser::IfStatContext *ctx) override;
	Object visitFuncname(LuaParser::FuncnameContext *ctx) override;
	Object visitNamelist(LuaParser::NamelistContext *ctx) override;
	Object visitExplist(LuaParser::ExplistContext *ctx) override;
	Object visitExp(LuaParser::ExpContext *ctx) override;
	Object visitPrefixexp(LuaParser::PrefixexpContext *ctx) override;
	Object visitFunctioncall(LuaParser::FunctioncallContext *ctx) override;
	Object visitVarOrExp(LuaParser::VarOrExpContext *ctx) override;
	Object visitVar_(LuaParser::Var_Context *ctx) override;
	Object visitVarSuffix(LuaParser::VarSuffixContext *ctx) override;
	Object visitNameAndArgs(LuaParser::NameAndArgsContext *ctx) override;
	Object visitArgs(LuaParser::ArgsContext *ctx) override;
	Object visitFunctiondef(LuaParser::FunctiondefContext *ctx) override;
	Object visitFuncbody(LuaParser::FuncbodyContext *ctx) override;
	Object visitParlist(LuaParser::ParlistContext *ctx) override;
	Object visitOperatorOr(LuaParser::OperatorOrContext *ctx) override;
	Object visitOperatorAnd(LuaParser::OperatorAndContext *ctx) override;
	Object visitOperatorComparison(LuaParser::OperatorComparisonContext *ctx) override;
	Object visitOperatorAddSub(LuaParser::OperatorAddSubContext *ctx) override;
	Object visitOperatorMulDiv(LuaParser::OperatorMulDivContext *ctx) override;
	Object visitNumber(LuaParser::NumberContext *ctx) override;
	Object visitString(LuaParser::StringContext *ctx) override;
};
} // namespace frontend

#endif /* SEMANTICS_H_ */
