#ifndef EXPRESSIONGENERATOR_H_
#define EXPRESSIONGENERATOR_H_

#include "CodeGenerator.h"
#include "LuaBaseVisitor.h"

namespace backend { namespace compiler {

class ExpressionGenerator : public CodeGenerator
{
public:
    /**
     * Constructor.
     * @param parent the parent code generator.
     * @param compiler the compiler to use.
     */
    ExpressionGenerator(CodeGenerator *parent, Compiler *compiler)
        : CodeGenerator(parent, compiler) {}

    /**
     * Emit code for an expression.
     * @param ctx the ExpressionContext.
     */
    void emitExpression(LuaParser::ExpContext *ctx);

    /**
     * Emit code to load a scalar variable's value
     * or a structured variable's address.
     * @param variableNode the variable node.
     */
    Typespec *emitLoadVariable(LuaParser::Var_Context *varCtx);

    /**
     * Emit code to load an integer constant.
     * @parm intCtx the IntegerConstantContext.
     */
    void emitLoadIntegerConstant(LuaParser::NumberContext *intCtx);

private:

};

}} // namespace backend::compiler

#endif /* EXPRESSIONGENERATOR_H_ */
