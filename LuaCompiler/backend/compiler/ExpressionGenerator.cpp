#include "LuaBaseVisitor.h"
#include "antlr4-runtime.h"

#include "intermediate/symtab/Predefined.h"
#include "intermediate/type/Typespec.h"
#include "CodeGenerator.h"
#include "ExpressionGenerator.h"
#include "Compiler.h"

namespace backend { namespace compiler {

void ExpressionGenerator::emitExpression(LuaParser::ExpContext *ctx)
{
    //emitSimpleExpression(simpleCtx1);

    // More than one simple expression?
    if (ctx->children.size() > 1)
    {
        string op = ""; //= relOpCtx->getText();
        if (ctx->operatorComparison() != nullptr)
        	op = ctx->operatorComparison()->getText();

        Label *trueLabel = new Label();
        Label *exitLabel = new Label();


		//emitSimpleExpression(simpleCtx2);

		if      (op == "==" ) emit(IFEQ, trueLabel);
		else if (op == "~=") emit(IFNE, trueLabel);
		else if (op == "<" ) emit(IFLT, trueLabel);
		else if (op == "<=") emit(IFLE, trueLabel);
		else if (op == ">" ) emit(IFGT, trueLabel);
		else if (op == ">=") emit(IFGE, trueLabel);


        emit(ICONST_0); // false
        emit(GOTO, exitLabel);
        emitLabel(trueLabel);
        emit(ICONST_1); // true
        emitLabel(exitLabel);

        localStack->decrease(1);  // only one branch will be taken
    }
}

Typespec *ExpressionGenerator::emitLoadVariable(LuaParser::Var_Context *varCtx, SymtabEntry *programId)
{
    SymtabEntry *variableId = programId->getRoutineSymtab()->lookup(varCtx->getText());
    Typespec *variableType = variableId->getType();

    // Scalar value or structure address.
    CodeGenerator::emitLoadValue(variableId);

    return variableType;
}

void ExpressionGenerator::emitLoadIntegerConstant(LuaParser::NumberContext *intCtx)
{
    int value = stoi(intCtx->getText());
    emitLoadConstant(value);
}

}} // namespace backend::compiler
