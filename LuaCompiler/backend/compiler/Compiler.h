#ifndef COMPILER_H_
#define COMPILER_H_

#include "LuaBaseVisitor.h"
#include "antlr4-runtime.h"

#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/type/Typespec.h"
#include "ProgramGenerator.h"
#include "StatementGenerator.h"
#include "ExpressionGenerator.h"

namespace backend { namespace compiler {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

class CodeGenerator;

class Compiler : public LuaBaseVisitor
{
private:
    SymtabEntry *programId;  // symbol table entry of the program name
    string programName;      // the program name

    CodeGenerator       *code;            // base code generator
    ProgramGenerator    *programCode;     // program code generator
    StatementGenerator  *statementCode;   // statement code generator
    ExpressionGenerator *expressionCode;  // expression code generator

public:
    /**
     * Constructor for the base compiler.
     * @param programId the symtab entry for the program name.
     */
    Compiler(SymtabEntry *programId)
        : programId(programId), programName(programId->getName()),
          code(new CodeGenerator(programName, "j", this)),
          programCode(nullptr), statementCode(nullptr),
          expressionCode(nullptr) {}

    /**
     * Constructor for child compilers of procedures and functions.
     * @param parent the parent compiler.
     */
    Compiler(Compiler *parent)
        : programId(parent->programId), programName(parent->programName),
          code(parent->code), programCode(parent->programCode),
          statementCode(nullptr), expressionCode(nullptr) {}

    /**
     * Get the name of the object (Jasmin) file.
     * @return the file name.
     */
    string getObjectFileName() { return code->getObjectFileName(); }
	Object visitChunk(LuaParser::ChunkContext *ctx) override;
	Object visitBlock(LuaParser::BlockContext *ctx) override;
	Object visitStat(LuaParser::StatContext *ctx) override;
	Object visitRetstat(LuaParser::RetstatContext *ctx) override;
	Object visitAssignStat(LuaParser::AssignStatContext *ctx) override;
	Object visitRepeatStat(LuaParser::RepeatStatContext *ctx) override;
	Object visitPrintStat(LuaParser::PrintStatContext *ctx) override;
	Object visitIfStat(LuaParser::IfStatContext *ctx) override;
	Object visitExplist(LuaParser::ExplistContext *ctx) override;
	Object visitExp(LuaParser::ExpContext *ctx) override;
	Object visitPrefixexp(LuaParser::PrefixexpContext *ctx) override;
	Object visitFunctioncall(LuaParser::FunctioncallContext *ctx) override;
	Object visitVarOrExp(LuaParser::VarOrExpContext *ctx) override;
	Object visitVar_(LuaParser::Var_Context *ctx) override;
	Object visitNameAndArgs(LuaParser::NameAndArgsContext *ctx) override;
	Object visitParlist(LuaParser::ParlistContext *ctx) override;
	Object visitNumber(LuaParser::NumberContext *ctx) override;
	Object visitString(LuaParser::StringContext *ctx) override;
private:
    /**
     * Create new child code generators.
     * @param parentGenerator the parent code generator.
     */
    void createNewGenerators(CodeGenerator *parentGenerator)
    {
        programCode    = new ProgramGenerator(parentGenerator, this, programId);
        statementCode  = new StatementGenerator(programCode, this);
        expressionCode = new ExpressionGenerator(programCode, this);
    }
};

}}  // namespace backend::compiler

#endif /* COMPILER_H_ */
