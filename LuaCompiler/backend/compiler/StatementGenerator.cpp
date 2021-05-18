#include <string>
#include <vector>
#include <map>

#include "LuaBaseVisitor.h"
#include "antlr4-runtime.h"
#include "intermediate/symtab/Predefined.h"
#include "CodeGenerator.h"
#include "Compiler.h"
#include "StatementGenerator.h"


namespace backend { namespace compiler {

using namespace std;
using namespace intermediate;


void StatementGenerator::emitAssignment(LuaParser::AssignStatContext *ctx)
{
	emitComment("ASSIGNMENT");
    LuaParser::ExpContext *exprCtx = ctx->exp();
    SymtabEntry *varId = ctx->var_()->entry;

    // Emit code to evaluate the expression.
    compiler->visit(exprCtx);

    // Emit code to store the expression value into the target variable.
    emitStoreValue(varId, Predefined::numberType);
}

void StatementGenerator::emitIf(LuaParser::IfStatContext *ctx)
{
	emitComment("IF");
	int num_of_expressions = ctx->exp().size();
	bool hasElse = (ctx->block().size() == num_of_expressions+1);

	Label *trueLabel = new Label();
	Label *falseLabel = new Label();

	for (int i=0; i<num_of_expressions; i++){
		if (i>0)
			emitComment("ELSE IF");
		compiler->visit(ctx->exp(i));
		emit(IFEQ, falseLabel);
		compiler->visit(ctx->block(i));
		emitLabel(falseLabel);
	}
	if (hasElse){
		emitComment("ELSE");
		emit(GOTO, trueLabel);
		emitLabel(falseLabel);
		compiler->visit(ctx->block(num_of_expressions));
		emitLabel(trueLabel);
	}
}


void StatementGenerator::emitRepeat(LuaParser::RepeatStatContext *ctx)
{
	emitComment("REPEAT");
    Label *loopTopLabel  = new Label();
    Label *loopExitLabel = new Label();

    emitLabel(loopTopLabel);

    compiler->visitBlock(ctx->block());
    emitComment("UNTIL");
    compiler->visitExp(ctx->exp());
    emit(IFNE, loopExitLabel);
    emit(GOTO, loopTopLabel);

    emitLabel(loopExitLabel);
}

void StatementGenerator::emitFunctionCall(LuaParser::FunctioncallContext *ctx, SymtabEntry* functionId)
{
	emitComment("FUNCTION CALL");
	string name = ctx->varOrExp()->var_()->getText();
	bool no_args = ctx->nameAndArgs(0)->args()->explist() == nullptr;
	string call = programName + "/" + name;

	if(no_args){
		call = call + "()";
		call = call + typeDescriptor(functionId);
		cout << "invoking static\n";
		emit(INVOKESTATIC, call);
		return;
	}


	emitCall(functionId);
}

void StatementGenerator::emitCall(SymtabEntry *routineId)
{
	string name = routineId->getName();
	string call = programName + "/" + name + "(";
	vector<SymtabEntry *> *parmIds = routineId->getRoutineParameters();

    if (parmIds != nullptr)
    {
        for (long unsigned int i=0; i< parmIds->size(); i++)
        {
            call += typeDescriptor(routineId);
        }
    }

    call = call + ")";
    call = call + typeDescriptor(routineId);

    emit(INVOKESTATIC, call);
}

void StatementGenerator::emitWrite(LuaParser::PrintStatContext *ctx)
{
	emitComment("PRINT");
    emitWrite(ctx->printArguments());
}

void StatementGenerator::emitWrite(LuaParser::PrintArgumentsContext *argsCtx)
{
    emit(GETSTATIC, "java/lang/System/out", "Ljava/io/PrintStream;");

    // WRITELN with no arguments.
    if (argsCtx == nullptr)
    {
        emit(INVOKEVIRTUAL, "java/io/PrintStream.println()V");
        localStack->decrease(1);
    }

    // Generate code for the arguments.
    else
    {
        string format;
        int exprCount = createWriteFormat(argsCtx, format);

        // Load the format string.
        emit(LDC, format);

        // Emit the arguments array.
       if (exprCount > 0)
        {
            emitArgumentsArray(argsCtx, exprCount);

            emit(INVOKEVIRTUAL,
                        string("java/io/PrintStream/printf(Ljava/lang/String;")
                      + string("[Ljava/lang/Object;)")
                      + string("Ljava/io/PrintStream;"));
            localStack->decrease(2);
            emit(POP);
        }
        else
        {
            emit(INVOKEVIRTUAL,
                 "java/io/PrintStream/print(Ljava/lang/String;)V");
            localStack->decrease(2);
        }
    }
}

int StatementGenerator::createWriteFormat(LuaParser::PrintArgumentsContext *argsCtx, string& format)
{
    int exprCount = 0;
    format += "\"";

    Typespec *type = Predefined::nilType;
	string argText = argsCtx->getText();

	// Append any literal strings.
	if (argText[0] == '\'') format += convertString(argText, true);

	// For any other expressions, append a field specifier.
	else
	{
		exprCount++;
		format.append("%");
		string typeFlag = type == Predefined::numberType ? "d"
						: type == Predefined::boolType    ? "b"
						: type == Predefined::stringType ? "s"
						:                                  "n";
		format += typeFlag;
    }

    format += "\\n\"";

    return exprCount;
}

void StatementGenerator::emitArgumentsArray(LuaParser::PrintArgumentsContext *argsCtx, int exprCount)
{
    // Create the arguments array.
    emitLoadConstant(exprCount);
    emit(ANEWARRAY, "java/lang/Object");

    int index = 0;
	string argText = argsCtx->getText();
	LuaParser::ExpContext *exprCtx = argsCtx->exp(0);

	// Skip string constants, which were made part of
	// the format string.
	if (argText[0] != '\'')
	{
		emit(DUP);
		emitLoadConstant(index++);

		compiler->visit(exprCtx);

		// Store the value into the array.
		emit(AASTORE);
	}
}
}
}// namespace backend::compiler
