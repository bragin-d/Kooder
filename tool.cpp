#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

// FIX 2: Define the option category as a static variable.
// This gives it a permanent address (making it an "lvalue").
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// FIX 1: Use DeclarationMatcher, not StatementMatcher.
// A function definition is a "Decl", not a "Stmt".
DeclarationMatcher FunctionMatcher = functionDecl(
    unless(isExpansionInSystemHeader())
).bind("func"); // Bind the matched node to the name "func"

// A MatchCallback that will be called for each match found.
class FunctionPrinter : public MatchFinder::MatchCallback {
public:
    // This function is called on every match.
    virtual void run(const MatchFinder::MatchResult &Result) {
        // Get the node that was bound to "func".
        const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

        if (Func) {
            // Check if it's a "real" function (not just a declaration)
            if (Func->isThisDeclarationADefinition()) {
                llvm::outs() << "Found function: " << Func->getNameAsString() << "\n";
            }
        }
    }
};

// Main function for our tool
int main(int argc, const char **argv) {
    // Parse the command-line arguments.
    // We now pass our static MyToolCategory variable.
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    // Create our printer and add the matcher to it.
    FunctionPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(FunctionMatcher, &Printer);

    // Run the tool.
    return Tool.run(newFrontendActionFactory(&Finder).get());
}