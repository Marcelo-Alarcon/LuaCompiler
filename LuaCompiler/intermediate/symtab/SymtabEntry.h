#ifndef SYMTABENTRY_H_
#define SYMTABENTRY_H_

#include <string>
#include <vector>
#include "antlr4-runtime.h"
#include "../../Object.h"

namespace intermediate { namespace type {
    class Typespec;
}};

namespace intermediate { namespace symtab {

using namespace std;
using intermediate::type::Typespec;

class Symtab;
class SymtabEntry;

enum class Kind
{
    CONSTANT, TYPE, VARIABLE, VALUE_PARAMETER, FUNCTION, CHUNK, UNDEFINED
};

static const string KIND_STRINGS[] =
{
    "constant", "type", "variable", "value parameter", "function", "chunk", "undefined"
};

constexpr Kind CONSTANT             = Kind::CONSTANT;
constexpr Kind TYPE					= Kind::TYPE;
constexpr Kind VARIABLE             = Kind::VARIABLE;
constexpr Kind VALUE_PARAMETER		= Kind::VALUE_PARAMETER;
constexpr Kind FUNCTION             = Kind::FUNCTION;
constexpr Kind CHUNK				= Kind::CHUNK;
constexpr Kind UNDEFINED            = Kind::UNDEFINED;

enum class Routine
{
    DECLARED, PRINT
};

constexpr Routine DECLARED	    = Routine::DECLARED;
constexpr Routine PRINT       	= Routine::PRINT;

class SymtabEntry
{
private:

    union EntryInfo
    {
        struct
        {
        	Object *value;
        } data;

        struct
        {
        public:
            Routine code;                        // routine code
            Symtab *symtab;                      // routine's symbol table
            vector<SymtabEntry *> *parameters;   // routine's formal parameters
            vector<SymtabEntry *> *subroutines;  // symtab entries of subroutines
            Object *executable;                  // routine's executable code
        } routine;
    };

    string name;              // identifier name
    Kind kind;                // what kind of identifier
    Symtab   *symtab;         // parent symbol table
    Typespec *typespec;       // type specification
    int slotNumber;           // local variables array slot number
    vector<int> lineNumbers;  // source line numbers
    EntryInfo info;           // entry information

public:
    /**
     * Constructor.
     * @param name the name of the entry.
     * @param kind the kind of entry.
     * @param symTab the symbol table that contains this entry.
     */
    SymtabEntry(const string name, const Kind kind, Symtab *symtab)
        : name(name), kind(kind), symtab(symtab), typespec(nullptr),
          slotNumber(0)
    {
        switch (kind)
        {
            case Kind::VARIABLE:

                info.data.value = nullptr;
                break;

            case Kind::FUNCTION:
            case Kind::CHUNK:
                info.routine.symtab = nullptr;
                info.routine.parameters  = new vector<SymtabEntry *>();
                info.routine.subroutines = new vector<SymtabEntry *>();
                break;

            default: break;
        }
    }

    /**
     * Destructor.
     */
    virtual ~SymtabEntry() {}

    /**
     * Get the name of the entry.
     * @return the name.
     */
    string getName() const { return name; }

    /**
     * Get the kind of entry.
     * @return the kind.
     */
    Kind getKind() const { return kind; }

    /**
     * Set the kind of entry.
     * @param kind the kind to set.
     */
    void setKind(Kind kind) { this->kind = kind; }

    /**
     * Getter.
     * @return the symbol table that contains this entry.
     */
    Symtab *getSymtab() const { return symtab; }

    /**
     * Get the slot number of the local variables array.
     * @return the number.
     */
    int getSlotNumber() const { return slotNumber; }

    /**
     * Set the slot number of the local variables array.
     * @param slotNumber the number to set.
     */
    void setSlotNumber(int slotNumber) { this->slotNumber = slotNumber; }

    /**
     * Getter.
     * @return the type specification.
     */
    Typespec *getType() const { return typespec; }

    /**
     * Setter.
     * @param typespec the type specification to set.
     */
    void setType(Typespec *typespec) { this->typespec = typespec; }

    /**
     * Getter.
     * @return the list of source line numbers.
     */
    vector<int> *getLineNumbers() { return &lineNumbers; }

    /**
     * Append a source line number to the entry.
     * @param _number the line number to append.
     */
    void appendLineNumber(const int _number)
    {
        lineNumbers.push_back(_number);
    }

    /**
     * Get the data value stored with this entry.
     * @return the data value.
     */
    Object getValue() const { return *(info.data.value); }

    /**
     * Set the data value into this entry.
     * @parm value the value to set.
     */
    void setValue(Object value) { info.data.value = new Object(value); }

    /**
     * Get the routine code.
     * @return the code.
     */
    Routine getRoutineCode() const { return info.routine.code; }

    /**
     * Set the routine code.
     * @parm code the code to set.
     */
    void setRoutineCode(const Routine code) { info.routine.code = code;}

    /**
     * Get the routine's symbol table.
     * @return the symbol table.
     */
    Symtab *getRoutineSymtab() const { return info.routine.symtab; }

    /**
     * Set the routine's symbol table.
     * @parm symtab the symbol table to set.
     */
    void setRoutineSymtab(Symtab *symtab) { info.routine.symtab = symtab; }

    /**
     * Get the vector of symbol table entries of the routine's formal parameters.
     * @return the vector.
     */
    vector<SymtabEntry *> *getRoutineParameters() const
    {
        return info.routine.parameters;
    }

    /**
     * Set the vector symbol table entries of parameters of the routine.
     * @parm parameters the vector to set.
     */
    void setRoutineParameters(vector<SymtabEntry *> *parameters)
    {
        info.routine.parameters = parameters;
    }

    /**
     * Get the vector of symbol table entries of the nested subroutines.
     * @return the vector.
     */
    vector<SymtabEntry *> *getSubroutines() const
    {
        return info.routine.subroutines;
    }

    /**
     * Append to the arraylist of symbol table entries of the nested subroutines.
     * @parm subroutineId the symbol table entry of the subroutine to append.
     */
    void appendSubroutine(SymtabEntry *subroutineId)
    {
        info.routine.subroutines->push_back(subroutineId);
    }

    /**
     * Get the routine's executable code.
     * @return the executable code.
     */
    Object getExecutable() const { return *(info.routine.executable); }

    /**
     * Set the routine's executable code.
     * @parm executable the executable code to set.
     */
    void setExecutable(Object executable)
    {
        info.routine.executable = new Object(executable);
    }
};

}}  // namespace intermediate::symtab

#endif /* SYMTABENTRY_H_ */
