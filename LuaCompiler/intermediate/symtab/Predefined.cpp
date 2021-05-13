#include <vector>

#include "antlr4-runtime.h"

#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/symtab/Predefined.h"
#include "intermediate/type/Typespec.h"
#include "Predefined.h"

namespace intermediate { namespace symtab {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

// Predefined types.
intermediate::type::Typespec *Predefined::numberType;
intermediate::type::Typespec *Predefined::boolType;
intermediate::type::Typespec *Predefined::nilType;
intermediate::type::Typespec *Predefined::stringType;
intermediate::type::Typespec *Predefined::undefinedType;

// Predefined identifiers.
SymtabEntry *Predefined::numberId;
SymtabEntry *Predefined::nilId;
SymtabEntry *Predefined::stringId;
SymtabEntry *Predefined::falseId;
SymtabEntry *Predefined::trueId;
SymtabEntry *Predefined::printId;



void Predefined::initialize(SymtabStack *symtabStack)
{
    initializeTypes(symtabStack);
    initializeConstants(symtabStack);
    initializeStandardRoutines(symtabStack);
}

void Predefined::initializeTypes(SymtabStack *symtabStack)
{
    // Type integer.
    numberId = symtabStack->enterLocal("number", TYPE);
    numberType = new Typespec();
    numberType->setIdentifier(numberId);
    numberId->setType(numberType);
}

void Predefined::initializeConstants(SymtabStack *symtabStack)
{
    // Boolean enumeration constant false.
    falseId = symtabStack->enterLocal("false", CONSTANT);
    falseId->setType(boolType);
    falseId->setValue(0);

    // Boolean enumeration constant true.
    trueId = symtabStack->enterLocal("true", CONSTANT);
    trueId->setType(boolType);
    trueId->setValue(1);

    // Add false and true to the boolean enumeration type.
    vector<SymtabEntry *> *constants = new vector<SymtabEntry *>;
    constants->push_back(falseId);
    constants->push_back(trueId);
}

void Predefined::initializeStandardRoutines(SymtabStack *symtabStack)
{
    printId    = enterStandard(symtabStack, FUNCTION, "print", PRINT);
}

SymtabEntry *Predefined::enterStandard(SymtabStack *symtabStack,
                                       const Kind kind, const string name,
                                       const Routine code)
{
    SymtabEntry *procId = symtabStack->enterLocal(name, kind);
    procId->setRoutineCode(code);

    return procId;
}

}}  // namespace wci::intermediate::symtabimpl
