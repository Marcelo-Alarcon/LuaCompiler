#include <vector>

#include "LuaBaseVisitor.h"
#include "antlr4-runtime.h"

#include "Directive.h"
#include "Instruction.h"
#include "Compiler.h"
#include "ProgramGenerator.h"

namespace backend { namespace compiler {

using namespace std;

void ProgramGenerator::emitProgram(LuaParser::ChunkContext *ctx)
{
    localVariables = new LocalVariables(programLocalsCount);

    emitDirective(CLASS_PUBLIC, programName);
    emitDirective(SUPER, "java/lang/Object");

    emitProgramVariables();
    emitInputScanner();
    emitConstructor();

    int i = 0;

    while (ctx->block()->stat(i)->functiondef() != nullptr){
    	emitRoutine(ctx->block()->stat(i)->functiondef());
    	i++;
    }

    emitMainMethod(ctx);
}

void ProgramGenerator::emitProgramVariables()
{
    // Runtime timer and standard in.
    Symtab *symtab = programId->getRoutineSymtab();
    vector<SymtabEntry *> ids = symtab->sortedEntries();

    emitLine();
    emitDirective(FIELD_PRIVATE_STATIC, "_sysin", "Ljava/util/Scanner;");

    // Loop over all the program's identifiers and
    // emit a .field directive for each variable.
    for (SymtabEntry *id : ids)
    {
        if (id->getKind() == VARIABLE)
        {
            emitDirective(FIELD_PRIVATE_STATIC, id->getName(),
                          typeDescriptor(id));
        }
    }
}

void ProgramGenerator::emitInputScanner()
{
    emitLine();
    emitComment("Runtime input scanner");
    emitDirective(METHOD_STATIC, "<clinit>()V");
    emitLine();

    emit(NEW, "java/util/Scanner");
    emit(DUP);
    emit(GETSTATIC, "java/lang/System/in Ljava/io/InputStream;");
    emit(INVOKESPECIAL, "java/util/Scanner/<init>(Ljava/io/InputStream;)V");
    emit(PUTSTATIC, programName + "/_sysin Ljava/util/Scanner;");
    emit(RETURN);

    emitLine();
    emitDirective(LIMIT_LOCALS, 0);
    emitDirective(LIMIT_STACK,  3);
    emitDirective(END_METHOD);

    localStack->reset();
}

void ProgramGenerator::emitConstructor()
{
    emitLine();
    emitComment("Main class constructor");
    emitDirective(METHOD_PUBLIC, "<init>()V");
    emitDirective(VAR, "0 is this L" + programName + ";");
    emitLine();

    emit(ALOAD_0);
    emit(INVOKESPECIAL, "java/lang/Object/<init>()V");
    emit(RETURN);

    emitLine();
    emitDirective(LIMIT_LOCALS, 1);
    emitDirective(LIMIT_STACK,  1);
    emitDirective(END_METHOD);

    localStack->reset();
}

void ProgramGenerator::emitMainMethod(LuaParser::ChunkContext *ctx)
{
    emitLine();
    emitComment("MAIN");
    emitDirective(METHOD_PUBLIC_STATIC,
                              "main([Ljava/lang/String;)V");

    emitMainPrologue(programId);
    emitLine();

    for (long unsigned int i=0; i<ctx->block()->children.size(); i++){
    	if (ctx->block()->stat(i)->functiondef() == nullptr)
    		compiler->visitChildren(ctx->block()->stat(i));
    }

    emitComment("END MAIN");
    emitMainEpilogue(programLocalsCount);
}

void ProgramGenerator::emitMainPrologue(SymtabEntry *programId)
{
    emitDirective(VAR, "0 is args [Ljava/lang/String;");
    emitDirective(VAR, "1 is _start Ljava/time/Instant;");
    emitDirective(VAR, "2 is _end Ljava/time/Instant;");
    emitDirective(VAR, "3 is _elapsed J");

    // Runtime timer.
    emitLine();
    emit(INVOKESTATIC, "java/time/Instant/now()Ljava/time/Instant;");
    localStack->increase(1);
    emit(ASTORE_1);
}

void ProgramGenerator::emitMainEpilogue(int max_local_vars)
{
    // Print the execution time.
    emitLine();
    emit(INVOKESTATIC, "java/time/Instant/now()Ljava/time/Instant;");
    localStack->increase(1);
    emit(ASTORE_2);
    emit(ALOAD_1);
    emit(ALOAD_2);
    emit(INVOKESTATIC,
         string("java/time/Duration/between(Ljava/time/temporal/Temporal;") +
         string("Ljava/time/temporal/Temporal;)Ljava/time/Duration;"));
    localStack->decrease(1);
    emit(INVOKEVIRTUAL, "java/time/Duration/toMillis()J");
    localStack->increase(1);
    emit(LSTORE_3);
    emit(GETSTATIC, "java/lang/System/out Ljava/io/PrintStream;");
    emit(LDC, "\"\\n[%,d milliseconds execution time.]\\n\"");
    emit(ICONST_1);
    emit(ANEWARRAY, "java/lang/Object");
    emit(DUP);
    emit(ICONST_0);
    emit(LLOAD_3);
    emit(INVOKESTATIC, "java/lang/Long/valueOf(J)Ljava/lang/Long;");
    emit(AASTORE);
    emit(INVOKEVIRTUAL,
         string("java/io/PrintStream/printf(Ljava/lang/String;") +
         string("[Ljava/lang/Object;)Ljava/io/PrintStream;"));
    localStack->decrease(2);
    emit(POP);

    emitLine();
    emit(RETURN);
    emitLine();


    emitDirective(LIMIT_LOCALS, localVariables->count() + max_local_vars);
    emitDirective(LIMIT_STACK,  localStack->capacity());
    emitDirective(END_METHOD);

    close();  // the object file
}

void ProgramGenerator::emitRoutine(LuaParser::FunctiondefContext *ctx)
{
	string routineName = ctx->funcname()->getText();
    SymtabEntry *routineId = programId->getRoutineSymtab()->lookup(routineName);
    Symtab *routineSymtab = routineId->getRoutineSymtab();

    emitRoutineHeader(routineId);
    emitRoutineLocals(routineId);
    localVariables = new LocalVariables(routineSymtab->getMaxSlotNumber());

    // Emit code for the compound statement.
    LuaParser::BlockContext *blockCtx = (LuaParser::BlockContext *) routineId->getExecutable();
    compiler->visit(blockCtx);


    emitRoutineReturn(routineId);
    emitRoutineEpilogue();
}

void ProgramGenerator::emitRoutineHeader(SymtabEntry *routineId)
{
    string routineName = routineId->getName();

    string header(routineName + "(");
    header += ")" + typeDescriptor(routineId);
    emitLine();
    emitComment("FUNCTION " + routineName);
    emitDirective(METHOD_PRIVATE_STATIC, header);

}

void ProgramGenerator::emitRoutineLocals(SymtabEntry *routineId)
{
    Symtab *symtab = routineId->getRoutineSymtab();
    vector<SymtabEntry *> ids = symtab->sortedEntries();

    emitLine();

    // Loop over all the routine's identifiers and
    // emit a .var directive for each variable and formal parameter.
    for (SymtabEntry *id : ids)
    {
        Kind kind = id->getKind();

        if ((kind == VARIABLE) || (kind == TYPE))
        {
            int i = 0;
            emitDirective(VAR, (i++) + " is " + id->getName(),
                          typeDescriptor(id));
        }
    }
}

void ProgramGenerator::emitRoutineReturn(SymtabEntry *routineId)
{
    emitLine();

    // Function: Return the value in the implied function variable.
    if (routineId->getKind() == FUNCTION)
    {
        Typespec *type = routineId->getType();

        // Get the slot number of the function variable.
        string varName = routineId->getName();
        SymtabEntry *varId = routineId->getRoutineSymtab()->lookup(varName);
        emitStoreLocal(type, varId->getSlotNumber());
        emitLoadLocal(type, varId->getSlotNumber());
        emitReturnValue(type);
    }

    // Procedure: Just return.
    else emit(RETURN);
}

void ProgramGenerator::emitRoutineEpilogue()
{
    emitLine();
    emitDirective(LIMIT_LOCALS, localVariables->count());
    emitDirective(LIMIT_STACK,  localStack->capacity());
    emitDirective(END_METHOD);
}

}} // namespace backend::compiler
