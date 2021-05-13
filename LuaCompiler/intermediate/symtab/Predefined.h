#ifndef PREDEFINED_H_
#define PREDEFINED_H_

#include <vector>

#include "../../Object.h"
#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/type/Typespec.h"

namespace intermediate { namespace symtab {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

class Predefined
{
public:
    // Predefined types.
    static Typespec *numberType;
    static Typespec *nilType;
    static Typespec *stringType;
    static Typespec *undefinedType;
    static Typespec *boolType;

    // Predefined identifiers.
    static SymtabEntry *numberId;
    static SymtabEntry *nilId;
    static SymtabEntry *stringId;
    static SymtabEntry *falseId;
    static SymtabEntry *trueId;
    static SymtabEntry *printId;

    /**
     * Initialize a symbol table stack with predefined identifiers.
     * @param symTab the symbol table stack to initialize.
     */
    static void initialize(SymtabStack *symtabStack);

private:
    /**
     * Initialize the predefined types.
     * @param symtabStack the symbol table stack to initialize.
     */
    static void initializeTypes(SymtabStack *symtabStack);

    /**
     * Initialize the predefined constant.
     * @param symtabStack the symbol table stack to initialize.
     */
    static void initializeConstants(SymtabStack *symtabStack);

    /**
     * Initialize the standard procedures and functions.
     * @param symtabStack the symbol table stack to initialize.
     */
    static void initializeStandardRoutines(SymtabStack *symtabStack);

    /**
     * Enter a standard procedure or function into the symbol table stack.
     * @param symTabStack the symbol table stack to initialize.
     * @param defn either PROCEDURE or FUNCTION.
     * @param name the procedure or function name.
     */
    static SymtabEntry *enterStandard(SymtabStack *symtabStack,
                                      const Kind kind, const string name,
                                      const Routine code);
};

}}  // namespace intermediate::symtab

#endif /* PREDEFINED_H_ */
