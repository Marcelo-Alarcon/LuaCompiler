/**
 * <h1>CrossReferencer</h1>
 *
 * <p>Generate a cross-reference listing.</p>
 *
 * <p>Copyright (c) 2020 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <iostream>
#include <string>
#include <vector>

#include "antlr4-runtime.h"

#include "CrossReferencer.h"
#include "intermediate/type/Typespec.h"
#include "intermediate/symtab/SymtabStack.h"
#include "intermediate/symtab/Symtab.h"
#include "intermediate/symtab/SymtabEntry.h"
#include "intermediate/symtab/Predefined.h"

namespace intermediate { namespace util {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

const int CrossReferencer::NAME_WIDTH = 16;

const string CrossReferencer::NAME_FORMAT = string("%-") +
                                            to_string(NAME_WIDTH) +
                                            string("s");
const string CrossReferencer::NUMBERS_LABEL = " Line numbers    ";
const string CrossReferencer::NUMBERS_UNDERLINE = " ------------    ";
const string CrossReferencer::NUMBER_FORMAT = " %03d";
const string CrossReferencer::ENUM_CONST_FORMAT = string("%") +
                                                  to_string(NAME_WIDTH) +
                                                  string("s = %s");

const int CrossReferencer::LABEL_WIDTH  = NUMBERS_LABEL.length();
const int CrossReferencer::INDENT_WIDTH = NAME_WIDTH + LABEL_WIDTH;

const string CrossReferencer::INDENT = string(INDENT_WIDTH, ' ');

void CrossReferencer::print(const SymtabStack *symtabStack) const
{
    cout << "\n===== CROSS-REFERENCE TABLE =====" << endl;

    SymtabEntry *programId = symtabStack->getProgramId();
    if (programId != nullptr)
    	printRoutine(programId);
    else
    	cout << "\tProgramID is null\n";
}

void CrossReferencer::printRoutine(SymtabEntry *routineId) const
{
    Kind kind = routineId->getKind();
    cout << endl << "*** " << KIND_STRINGS[(int) kind]
         << " " << routineId->getName() << " ***" << endl;
    printColumnHeadings();

    // Print the entries in the routine's symbol table.
    Symtab *symtab = routineId->getRoutineSymtab();
    vector<Typespec *> newRecordTypes;
    printSymtab(symtab, newRecordTypes);

    // Print any procedures and functions defined in the routine.
    vector<SymtabEntry *> *routineIds = routineId->getSubroutines();

    if (routineIds != nullptr)
    {
        for (SymtabEntry *routineId : *routineIds) printRoutine(routineId);
    }
}

void CrossReferencer::printColumnHeadings() const
{
    cout << endl;
    printf(NAME_FORMAT.c_str(), "Identifier");
    cout << NUMBERS_LABEL     << "Type specification" << endl;
    printf(NAME_FORMAT.c_str(), "----------");
    cout << NUMBERS_UNDERLINE << "------------------" << endl;
}

void CrossReferencer::printSymtab(Symtab *symtab,
                                  vector<Typespec *>& recordTypes) const
{
    // Loop over the sorted list of symbol table entries.
    vector<SymtabEntry *> sorted = symtab->sortedEntries();
    for (SymtabEntry *entry : sorted)
    {
        vector<int> *line_numbers = entry->getLineNumbers();

        // For each entry, print the identifier name
        // followed by the line numbers.
        printf(NAME_FORMAT.c_str(), entry->getName().c_str());
        if (line_numbers->size() > 0)
        {
            for (int line_number : *line_numbers)
            {
                printf(NUMBER_FORMAT.c_str(), line_number);
            }
        }

        // Print the symbol table entry.
        cout << endl;
        printEntry(entry, recordTypes);
    }
}

void CrossReferencer::printEntry(SymtabEntry *entry,
                                 vector<Typespec *>& recordTypes) const
{
    Kind kind = entry->getKind();
    int nestingLevel = entry->getSymtab()->getNestingLevel();
    cout << INDENT << "Defined as: " << KIND_STRINGS[(int) kind] << endl;
    cout << INDENT << "Scope nesting level: " << nestingLevel << endl;

    // Print the type specification.
    Typespec *type = entry->getType();
    printType(type);
}

void CrossReferencer::printType(Typespec *typespec) const
{
    if (typespec != nullptr)
    {
        SymtabEntry *type_id = typespec->getIdentifier();
        string type_name = type_id != nullptr ? type_id->getName()
                                              : "<unnamed>";

        cout << INDENT << "Type form = " << typespec->baseType()
             << ", Type id = " << type_name << endl;
    }
}

string CrossReferencer::toString(Object value, Typespec *type) const
{
    if      (type == Predefined::numberType)    return to_string(value.as<int>());
    else if (type == Predefined::boolType)      return to_string(value.as<bool>());
    else if (type == Predefined::stringType)    return "'" + *(value.as<string *>()) + "'";
    else  /* nil */          					return "nil";
}

}}  // namespace intermediate ::util
