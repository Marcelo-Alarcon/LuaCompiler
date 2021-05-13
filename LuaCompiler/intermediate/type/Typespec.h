#ifndef TYPESPEC_H_
#define TYPESPEC_H_

#include <string>
#include <vector>

#include "../../Object.h"
#include "intermediate/symtab/Symtab.h"

namespace intermediate { namespace type {

using namespace std;
using namespace intermediate::symtab;

class Typespec;

class Typespec
{
private:
    SymtabEntry *identifier;

public:
    /**
     * Constructor.
     * @param form the type form.
     */
    Typespec() : identifier(nullptr) {}

    /**
     * Destructor.
     */
    virtual ~Typespec() {}

    /**
     * Get the type identifier's symbol table entry.
     * @return the symbol table entry.
     */
    SymtabEntry *getIdentifier() const { return identifier; }


    Typespec *baseType()
        {
            return this;
        }

    /**
     * Set the type identifier's symbol table entry.
     * @param identifier the symbol table entry.
     */
    void setIdentifier(SymtabEntry *identifier) { this->identifier = identifier; }

};

}}  // namespace intermediate::type

#endif /* TYPESPEC_H_ */
