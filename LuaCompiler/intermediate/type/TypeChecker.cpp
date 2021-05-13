#include "intermediate/symtab/Predefined.h"
#include "TypeChecker.h"
#include "Typespec.h"

namespace intermediate { namespace type {

using namespace std;
using namespace intermediate::symtab;
using namespace intermediate::type;

bool TypeChecker::isNumber(Typespec *typespec)
{
    return (typespec != nullptr) && (typespec == Predefined::numberType);
}

bool TypeChecker::areBothNumbers(Typespec *typespec1, Typespec *typespec2)
{
    return isNumber(typespec1) && isNumber(typespec2);
}


bool TypeChecker::isBoolean(Typespec *typespec)
{
    return (typespec != nullptr) && (typespec->baseType() == Predefined::boolType);
}

bool TypeChecker::areBothBoolean(Typespec *typespec1, Typespec *typespec2)
{
    return isBoolean(typespec1) && isBoolean(typespec2);
}

bool TypeChecker::isString(Typespec *typespec)
{
    return (typespec != nullptr) && (typespec->baseType() == Predefined::stringType);
}

bool TypeChecker::areBothString(Typespec *typespec1, Typespec *typespec2)
{
    return isString(typespec1) && isString(typespec2);
}

bool TypeChecker::areCompatible(Typespec *type1,
                                          Typespec *type2)
{
    if ((type1 == nullptr) || (type2 == nullptr))  return false;

    type1 = type1->baseType();
    type2 = type2->baseType();

    bool compatible = false;

    // Two identical scalar or enumeration types.
    if (type1 == type2)
        compatible = true;

    return compatible;
}

}}  // namespace :intermediate::typ
