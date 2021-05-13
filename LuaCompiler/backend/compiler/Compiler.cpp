#include "intermediate/symtab/Predefined.h"
#include "Compiler.h"

namespace backend { namespace compiler {

Object Compiler::visitChunk(LuaParser::ChunkContext *ctx){
	return nullptr;
}
Object Compiler::visitBlock(LuaParser::BlockContext *ctx){
	return nullptr;
}
Object Compiler::visitStat(LuaParser::StatContext *ctx){
	return nullptr;
}
Object Compiler::visitRetstat(LuaParser::RetstatContext *ctx){
	return nullptr;
}
Object Compiler::visitAssignStat(LuaParser::AssignStatContext *ctx){
	return nullptr;
}
Object Compiler::visitRepeatStat(LuaParser::RepeatStatContext *ctx){
	return nullptr;
}
Object Compiler::visitIfStat(LuaParser::IfStatContext *ctx){
	return nullptr;
}

Object Compiler::visitPrintStat(LuaParser::PrintStatContext *ctx) {
   return nullptr;
 }

Object Compiler::visitPrintArguments(LuaParser::PrintArgumentsContext *ctx) {
   return nullptr;
 }

Object Compiler::visitFuncname(LuaParser::FuncnameContext *ctx){
	return nullptr;
}

Object Compiler::visitNamelist(LuaParser::NamelistContext *ctx){
	return nullptr;
}
Object Compiler::visitExplist(LuaParser::ExplistContext *ctx){
	return nullptr;
}
Object Compiler::visitExp(LuaParser::ExpContext *ctx){
	return nullptr;
}
Object Compiler::visitPrefixexp(LuaParser::PrefixexpContext *ctx){
	return nullptr;
}
Object Compiler::visitFunctioncall(LuaParser::FunctioncallContext *ctx){
	return nullptr;
}
Object Compiler::visitVarOrExp(LuaParser::VarOrExpContext *ctx){
	return nullptr;
}
Object Compiler::visitVar_(LuaParser::Var_Context *ctx){
	return nullptr;
}
Object Compiler::visitVarSuffix(LuaParser::VarSuffixContext *ctx){
	return nullptr;
}
Object Compiler::visitNameAndArgs(LuaParser::NameAndArgsContext *ctx){
	return nullptr;
}
Object Compiler::visitArgs(LuaParser::ArgsContext *ctx){
	return nullptr;
}
Object Compiler::visitFunctiondef(LuaParser::FunctiondefContext *ctx){
	return nullptr;
}
Object Compiler::visitFuncbody(LuaParser::FuncbodyContext *ctx){
	return nullptr;
}
Object Compiler::visitParlist(LuaParser::ParlistContext *ctx){
	return nullptr;
}
Object Compiler::visitOperatorOr(LuaParser::OperatorOrContext *ctx){
	return nullptr;
}
Object Compiler::visitOperatorAnd(LuaParser::OperatorAndContext *ctx){
	return nullptr;
}
Object Compiler::visitOperatorComparison(LuaParser::OperatorComparisonContext *ctx){
	return nullptr;
}
Object Compiler::visitOperatorAddSub(LuaParser::OperatorAddSubContext *ctx){
	return nullptr;
}
Object Compiler::visitOperatorMulDiv(LuaParser::OperatorMulDivContext *ctx){
	return nullptr;
}
Object Compiler::visitNumber(LuaParser::NumberContext *ctx){
	return nullptr;
}
Object Compiler::visitString(LuaParser::StringContext *ctx){
	return nullptr;
}

}}  // namespace backend::compiler
