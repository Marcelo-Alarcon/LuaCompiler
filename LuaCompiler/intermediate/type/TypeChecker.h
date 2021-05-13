/**
 * <h1>TypeChecker</h1>
 *
 * <p>Perform type checking.</p>
 *
 * <p>Copyright (c) 2020 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef TYPECHECKER_H_
#define TYPECHECKER_H_

#include "intermediate/type/Typespec.h"

namespace intermediate { namespace type {

using namespace std;
using namespace intermediate::type;

class TypeChecker
{
public:
    static bool isNumber(Typespec *typespec);
    static bool areBothNumbers(Typespec *typespec1, Typespec *typespec2);

    static bool isBoolean(Typespec *typespec);
    static bool areBothBoolean(Typespec *typespec1, Typespec *typespec2);

    static bool isString(Typespec *typespec);
    static bool areBothString(Typespec *typespec1, Typespec *typespec2);

    static bool areCompatible(Typespec *typespec1, Typespec *typespec2);
};

}}  // namespace intermediate::type

#endif /* TYPECHECKER_H_ */
