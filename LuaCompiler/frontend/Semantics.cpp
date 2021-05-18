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
	visitVar_(ctx->var_());
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
	string funcName = ctx->varOrExp()->var_()->getText();
	SymtabEntry *func = symtabStack->lookup(funcName);
	Symtab *tmp = func->getRoutineSymtab();

	SymtabEntry *ptr = nullptr;
	vector<SymtabEntry *> entries = tmp->sortedEntries();

	for (long unsigned int i=0; i<entries.size(); i++){
		if (entries[i]->getName() == "testData")
			ptr = entries[i];
	}
	func->setRoutineParameters(ptr->getRoutineParameters());
	visitChildren(ctx);
	return nullptr;
}
Object Semantics::visitVarOrExp(LuaParser::VarOrExpContext *ctx){
	if (ctx->var_() != nullptr)
		visitVar_(ctx->var_());
	else {
		if (ctx->exp() != nullptr)
			visitExp(ctx->exp());
	}
	return nullptr;
}

Object Semantics::visitVar_(LuaParser::Var_Context *ctx){
	string name = ctx->NAME()->getText();
	SymtabEntry *varId = symtabStack->lookupLocal(name);

	int lineNum = ctx->getStart()->getLine();
	if (varId == nullptr){
		varId = symtabStack->enterLocal(name, VARIABLE);
		varId->setType(Predefined::numberType);
	}
	ctx->entry = varId;

	if (varId->getKind() == FUNCTION)
		return nullptr;

	varId->appendLineNumber(lineNum);
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
	LuaParser::ParlistContext *parameterList = ctx->funcbody()->parlist();

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
		vector<SymtabEntry *> parameterIds;

		if (parameterList != nullptr){

			for (unsigned long int i=0; i<parameterList->varlist()->var_().size(); i++){

				string name = parameterList->varlist()->var_(i)->getText();
				SymtabEntry *newEntry = symtabStack->enterLocal(name, VALUE_PARAMETER);
				newEntry->setType(Predefined::numberType);
				parameterList->varlist()->var_(i)->entry = newEntry;
				parameterIds.push_back(newEntry);
				newEntry->setSlotNumber(localSymtab->nextSlotNumber());

			}
			functionId->setRoutineParameters(&parameterIds);
		}

		SymtabEntry *assocVarId = symtabStack->enterLocal(functionName, VARIABLE);
		assocVarId->setSlotNumber(localSymtab->nextSlotNumber());
		assocVarId->setType(Predefined::numberType);

		visitChildren(ctx->funcbody()->block());
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
	visit(ctx->varlist());
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
