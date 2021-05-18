#include "intermediate/symtab/Predefined.h"
#include "LuaBaseVisitor.h"
#include "Compiler.h"

namespace backend { namespace compiler {

Object Compiler::visitChunk(LuaParser::ChunkContext *ctx){
	createNewGenerators(code);
	programCode->emitProgram(ctx);
	return nullptr;
}
Object Compiler::visitBlock(LuaParser::BlockContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Compiler::visitStat(LuaParser::StatContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Compiler::visitRetstat(LuaParser::RetstatContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Compiler::visitAssignStat(LuaParser::AssignStatContext *ctx){
	statementCode->emitAssignment(ctx);
	return nullptr;
}
Object Compiler::visitRepeatStat(LuaParser::RepeatStatContext *ctx){
	statementCode->emitRepeat(ctx);
	return nullptr;
}
Object Compiler::visitIfStat(LuaParser::IfStatContext *ctx){
	statementCode->emitIf(ctx);
	return nullptr;
}

Object Compiler::visitPrintStat(LuaParser::PrintStatContext *ctx) {
	statementCode->emitWrite(ctx);
	return nullptr;
}

Object Compiler::visitExp(LuaParser::ExpContext *ctx){
	expressionCode->emitExpression(ctx);
	return nullptr;
}

Object Compiler::visitExplist(LuaParser::ExplistContext *ctx){
	visitChildren(ctx);
	return nullptr;
}

Object Compiler::visitPrefixexp(LuaParser::PrefixexpContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Compiler::visitFunctioncall(LuaParser::FunctioncallContext *ctx){
	Symtab *tmp = programId->getRoutineSymtab();
	SymtabEntry *functionId = tmp->lookup(ctx->varOrExp()->var_()->getText());

	statementCode->emitFunctionCall(ctx, functionId);
	return nullptr;
}
Object Compiler::visitVarOrExp(LuaParser::VarOrExpContext *ctx){
	if (ctx->exp() != nullptr)
		visitExp(ctx->exp());
	else if (ctx->var_() != nullptr)
		visitVar_(ctx->var_());

	return nullptr;
}
Object Compiler::visitVar_(LuaParser::Var_Context *ctx){
	expressionCode->emitLoadVariable(ctx);
	return nullptr;
}
Object Compiler::visitNameAndArgs(LuaParser::NameAndArgsContext *ctx){
	visitChildren(ctx);
	return nullptr;
}

Object Compiler::visitParlist(LuaParser::ParlistContext *ctx){
	visitChildren(ctx);
	return nullptr;
}
Object Compiler::visitNumber(LuaParser::NumberContext *ctx){
	expressionCode->emitLoadConstant(stoi(ctx->getText()));
	return nullptr;
}
Object Compiler::visitString(LuaParser::StringContext *ctx){
	string jasminString = convertString(ctx->getText(), true);
	expressionCode->emitLoadConstant(jasminString);
	return nullptr;
}

}}  // namespace backend::compiler
