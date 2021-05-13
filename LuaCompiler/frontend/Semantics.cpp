#include <vector>
#include <set>

#include "antlr4-runtime.h"

#include "LuaBaseVisitor.h"
#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/Symtab.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/symtab/Predefined.h"
#include "intermediate/type/Typespec.h"
#include "intermediate/type/TypeChecker.h"
#include "intermediate/util/CrossReferencer.h"
#include "SemanticErrorHandler.h"
#include "Semantics.h"

namespace intermediate { namespace symtab {
    int Symtab::unnamedIndex = 0;
}}

namespace frontend {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;
using namespace intermediate::util;

Semantics::Semantics(string x)
{
    // Create and initialize the symbol table stack.
	programName = x;
	programId = nullptr;
    symtabStack = new SymtabStack();
    Predefined::initialize(symtabStack);

    typeTable = new map<string, Typespec *>();
    (*typeTable)["number"] = Predefined::numberType;
    (*typeTable)["nil"]    = Predefined::nilType;
    (*typeTable)["boolean"]    = Predefined::boolType;
    (*typeTable)["string"]  = Predefined::stringType;
}

Object Semantics::visitChunk(LuaParser::ChunkContext *ctx){
	programId = symtabStack->enterLocal(programName, CHUNK);
	programId->setRoutineSymtab(symtabStack->push());
	symtabStack->setProgramId(programId);
	symtabStack->getLocalSymtab()->setOwner(programId);
	visit(ctx->block());

	CrossReferencer crossReferencer;
	crossReferencer.print(symtabStack);

	return nullptr;
}
Object Semantics::visitBlock(LuaParser::BlockContext *ctx){
	visitChildren(ctx);
	return nullptr;
}

Object Semantics::visitStat(LuaParser::StatContext *ctx){
	visitChildren(ctx);
	return nullptr;
}

Object Semantics::visitRetstat(LuaParser::RetstatContext *ctx){
	return visit(ctx->exp());
}
Object Semantics::visitAssignStat(LuaParser::AssignStatContext *ctx){
	visit(ctx->var_());
	visit(ctx->exp());
	return nullptr;
}

Object Semantics::visitRepeatStat(LuaParser::RepeatStatContext *ctx){
	visit(ctx->exp());
	visit(ctx->block());
	return nullptr;
}
Object Semantics::visitIfStat(LuaParser::IfStatContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitPrintStat(LuaParser::PrintStatContext *ctx) {
	visitPrintArguments(ctx->printArguments());
	return nullptr;
 }

Object Semantics::visitPrintArguments(LuaParser::PrintArgumentsContext *ctx) {
	visitChildren(ctx);
	return nullptr;
 }

Object Semantics::visitFuncname(LuaParser::FuncnameContext *ctx){
	return ctx->getText();
}

Object Semantics::visitNamelist(LuaParser::NamelistContext *ctx){
	return ctx->getText();
}
Object Semantics::visitExplist(LuaParser::ExplistContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitExp(LuaParser::ExpContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitPrefixexp(LuaParser::PrefixexpContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitFunctioncall(LuaParser::FunctioncallContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitVarOrExp(LuaParser::VarOrExpContext *ctx){
	if (ctx->var_() != nullptr)
		visit(ctx->var_());
	else {
		if (ctx->exp() != nullptr)
			visit(ctx->exp());
	}
	return nullptr;
}
Object Semantics::visitVar_(LuaParser::Var_Context *ctx){
	string name = ctx->NAME()->getText();
	SymtabEntry *varId = symtabStack->lookupLocal(name);
	int lineNum = ctx->getStart()->getLine();

	if (varId == nullptr){
		varId = symtabStack->enterLocal(name, VARIABLE);
		varId->setType(Predefined::nilType);
	} else {
		if (varId->getKind() == FUNCTION)
			return nullptr;
	}
	varId->appendLineNumber(lineNum);
	return nullptr;
}

Object Semantics::visitVarSuffix(LuaParser::VarSuffixContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitNameAndArgs(LuaParser::NameAndArgsContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitArgs(LuaParser::ArgsContext *ctx){
	visit(ctx->explist());
	return nullptr;
}
Object Semantics::visitFunctiondef(LuaParser::FunctiondefContext *ctx){
	string functionName = ctx->funcname()->getText();
	SymtabEntry *functionId = symtabStack->lookupLocal(functionName);
	LuaParser::ParlistContext *parameters = ctx->funcbody()->parlist();

	if (functionId != nullptr){
		error.flag(REDECLARED_IDENTIFIER, ctx->getStart()->getLine(), functionName);
		return nullptr;
	} else {
		functionId = symtabStack->enterLocal(functionName, FUNCTION);
		functionId->setRoutineCode(DECLARED);

		SymtabEntry *parent = symtabStack->getLocalSymtab()->getOwner();

		if (parent !=  nullptr){
			parent->appendSubroutine(functionId);
		}

		functionId->setRoutineSymtab(symtabStack->push());
		Symtab *localSymtab = symtabStack->getLocalSymtab();
		localSymtab->setOwner(functionId);

		if (parameters != nullptr){
			vector<SymtabEntry *> parameterIds;

			for (unsigned long int i=0; i<parameters->namelist()->children.size(); i++){

				string name = parameters->namelist()->children[i]->getText();
				if (name != ","){
					SymtabEntry *newEntry = symtabStack->enterLocal(name, VALUE_PARAMETER);
					parameterIds.push_back(newEntry);
					newEntry->setSlotNumber(localSymtab->nextSlotNumber());
				}
			}
			functionId->setRoutineParameters(&parameterIds);
		}

		visit(ctx->funcbody());
		functionId->setExecutable(ctx->funcbody()->block());

		symtabStack->pop();
		return nullptr;
	}
}

Object Semantics::visitFuncbody(LuaParser::FuncbodyContext *ctx){
	visitChildren(ctx->block());
	return nullptr;
}
Object Semantics::visitParlist(LuaParser::ParlistContext *ctx){
	string params = visit(ctx->namelist());
	return nullptr;
}
Object Semantics::visitOperatorOr(LuaParser::OperatorOrContext *ctx){
	return nullptr;
}
Object Semantics::visitOperatorAnd(LuaParser::OperatorAndContext *ctx){
	return nullptr;
}
Object Semantics::visitOperatorComparison(LuaParser::OperatorComparisonContext *ctx){
	return nullptr;
}
Object Semantics::visitOperatorAddSub(LuaParser::OperatorAddSubContext *ctx){
	return nullptr;
}
Object Semantics::visitOperatorMulDiv(LuaParser::OperatorMulDivContext *ctx){
	return nullptr;
}
Object Semantics::visitNumber(LuaParser::NumberContext *ctx){
	return nullptr;
}
Object Semantics::visitString(LuaParser::StringContext *ctx){
	return nullptr;
}

} // namespace frontend
