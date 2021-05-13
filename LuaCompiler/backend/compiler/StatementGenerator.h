#ifndef STATEMENTGENERATOR_H_
#define STATEMENTGENERATOR_H_

#include <vector>
#include <map>

#include "CodeGenerator.h"

namespace backend { namespace compiler {

using namespace std;

class StatementGenerator : public CodeGenerator
{
public:
    /**
     * Constructor.
     * @param parent the parent code generator.
     * @param compiler the compiler to use.
     */
    StatementGenerator(CodeGenerator *parent, Compiler *compiler)
        : CodeGenerator(parent, compiler) {}

    /**
     * Emit code for an assignment statement.
     * @param ctx the AssignmentStatementContext.
     */
    void emitAssignment(LuaParser::AssignStatContext *ctx);

    /**
     * Emit code for an IF statement.
     * @param ctx the IfStatementContext.
     */
    void emitIf(LuaParser::IfStatContext *ctx);

    /*
     * Emit code for a REPEAT statement.
     * @param ctx the RepeatStatementContext.
     */
    void emitRepeat(LuaParser::RepeatStatContext *ctx);

    /**
     * Emit code for a function call statement.
     * @param ctx the FunctionCallContext.
     */
    void emitFunctionCall(LuaParser::FunctioncallContext *ctx, SymtabEntry *ptr);

    /**
     * Emit code for a WRITE statement.
     * @param ctx the WriteStatementContext.
     */
    void emitWrite(LuaParser::PrintStatContext *ctx);



private:
//    /**
//     * Emit a call to a procedure or a function.
//     * @param routineId the routine name's symbol table entry.
//     */
    void emitCall(SymtabEntry *routineId);

    /**
     * Emit code for a call to PRINT
     * @param argsCtx the print arguments context.

     */
    void emitWrite(LuaParser::PrintArgumentsContext *argsCtx);

    /**
     * Create the printf format string.
     * @param argsCtx the print arguments context.
     * @param format the format string to create.
     * @return the count of expression arguments.
     */
    int createWriteFormat(LuaParser::PrintArgumentsContext *argsCtx, string& format);

    /**
     * Emit the print arguments array.
     * @param argsCtx
     * @param exprCount
     */
    void emitArgumentsArray(LuaParser::PrintArgumentsContext *argsCtx, int exprCount);


};

}} // namespace backend::compiler

#endif /* STATEMENTGENERATOR_H_ */
