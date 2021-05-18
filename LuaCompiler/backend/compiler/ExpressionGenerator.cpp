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
    // More than one expression?
    if (ctx->children.size() > 1)
    {
        string op = "";
        if (ctx->operatorComparison() != nullptr){
        	emitComment(ctx->getText());
        	Label *trueLabel = new Label();
			Label *exitLabel = new Label();
			op = ctx->operatorComparison()->getText();
			compiler->visit(ctx->exp(0)); // LHS expression
			compiler->visit(ctx->exp(1)); // RHS expression

			if      (op == "==" ) emit(IF_ICMPEQ, trueLabel);
			else if (op == "~=")  emit(IF_ICMPNE, trueLabel);
			else if (op == "<" )  emit(IF_ICMPLT, trueLabel);
			else if (op == "<=")  emit(IF_ICMPLE, trueLabel);
			else if (op == ">" )  emit(IF_ICMPGT, trueLabel);
			else if (op == ">=")  emit(IF_ICMPGE, trueLabel);

			emit(ICONST_0); // false
			emit(GOTO, exitLabel);
			emitLabel(trueLabel);
			emit(ICONST_1); // true
			emitLabel(exitLabel);

			localStack->decrease(1);  // only one branch will be taken

        } else if (ctx->operatorAddSub() != nullptr){
        	op = ctx->operatorAddSub()->getText();
        	compiler->visit(ctx->exp(0)); // LHS expression
        	compiler->visit(ctx->exp(1)); // RHS expression

        	if (op == "+")
        		emit(IADD);
        	else if (op == "-")
        		emit(ISUB);

        } else if (ctx->operatorMulDiv() != nullptr){
        	op = ctx->operatorMulDiv()->getText();
        	compiler->visit(ctx->exp(0)); // LHS expression
        	compiler->visit(ctx->exp(1)); // RHS expression

        	if (op == "*")
        		emit(IMUL);
        	else if (op == "/")
        		emit(IDIV);
        }


    } else {
    	compiler->visitChildren(ctx);
    }
}

Typespec *ExpressionGenerator::emitLoadVariable(LuaParser::Var_Context *varCtx)
{
    SymtabEntry *variableId = varCtx->entry;
    Typespec *variableType = Predefined::numberType;

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
