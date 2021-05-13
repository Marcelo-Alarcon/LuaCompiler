#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

#include "antlr4-runtime.h"
#include "LuaLexer.h"
#include "LuaParser.h"

#include "frontend/Listing.h"
#include "frontend/SyntaxErrorHandler.h"
#include "frontend/Semantics.h"
#include "intermediate/symtab/Predefined.h"
#include "intermediate/type/Typespec.h"
#include "backend/compiler/Compiler.h"

using namespace std;
using namespace antlrcpp;
using namespace antlr4;

using namespace frontend;
using namespace intermediate::type;
using namespace intermediate::symtab;
using namespace backend::compiler;

int main(int argc, const char *args[])
{
    if (argc < 1)
    {
        cout << "USAGE: Lua sourceFileName" << endl;
        return -1;
    }

    string sourceFile = args[1];
    string sourceFileName = sourceFile.substr(0, sourceFile.size()-4);

    cout << "PASS 1: \n";
    // Generate a source file listing.
    Listing listing(sourceFile);

    ifstream ins;
    ins.open(args[1]);

    // Create the input stream.
    ANTLRInputStream input(ins);
    // Custom syntax error handler.
    SyntaxErrorHandler syntaxErrorHandler;

    // Create a lexer which scans the character stream
    // to create a token stream.
    LuaLexer lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&syntaxErrorHandler);
    CommonTokenStream tokens(&lexer);

    // Create a parser which parses the token stream.
    LuaParser parser(&tokens);

    // Pass 1: Check syntax and create the parse tree.
    tree::ParseTree *tree = parser.chunk();

    // Allow any syntax error messages to print.
	this_thread::sleep_for(chrono::milliseconds(100));
	int syntaxErrors = syntaxErrorHandler.getCount();
	if (syntaxErrors > 0)
	{
		printf("\nThere were %d syntax errors.\n", syntaxErrors);
		cout << "Object file not created or modified." << endl;
		return syntaxErrors;
	}

    // Pass 2: Create symbol tables and set parse tree node datatypes.
	cout << "\nPASS 2:";
	Semantics *pass2 = new Semantics(sourceFileName);
	pass2->visit(tree);

	int semanticErrors = pass2->getErrorCount();
	if (semanticErrors > 0)
	{
		cout << endl << "There were " << semanticErrors << " semantic errors." << " Object file not created or modified." << endl;
		return semanticErrors;
	}

	// Pass 3: Compile the Lua program.
	cout << "\nPASS 3: \n";
	SymtabEntry *programId = pass2->getProgramId();
	Compiler *pass3 = new Compiler(programId);
	pass3->visit(tree);

	cout << "Object file \"" << pass3->getObjectFileName() << "\" created." << endl;

    return 0;
}
