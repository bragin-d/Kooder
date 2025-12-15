#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// --- 1. Helper to store valid line ranges ---
struct ScopeScanner {
    std::vector<std::pair<unsigned, unsigned>> validRanges;

    // Reads the file as plain text to find //START and //END
    void scanFile(const std::string &filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        unsigned lineNum = 0;
        unsigned startLine = 0;
        bool insideScope = false;

        while (std::getline(file, line)) {
            lineNum++;
            // Simple string check for the comments
            if (line.find("// START") != std::string::npos) {
                startLine = lineNum;
                insideScope = true;
            }
            else if (line.find("// END") != std::string::npos && insideScope) {
                validRanges.push_back({startLine, lineNum});
                insideScope = false;
            }
        }
    }

    // Check if a specific line is inside any valid range
    bool isInScope(unsigned line) {
        for (const auto &range : validRanges) {
            if (line >= range.first && line <= range.second) {
                return true;
            }
        }
        return false;
    }
};

DeclarationMatcher FunctionMatcher = functionDecl(
    unless(isExpansionInSystemHeader())
).bind("func");

// --- 2. Modified Callback ---
class FunctionPrinter : public MatchFinder::MatchCallback {
    ScopeScanner &Scanner; // Reference to our scanner

public:
    explicit FunctionPrinter(ScopeScanner &S) : Scanner(S) {}

    virtual void run(const MatchFinder::MatchResult &Result) {
        const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

        if (Func && Func->isThisDeclarationADefinition()) {
            
            // Get the SourceManager to translate location -> line number
            SourceManager &SM = Result.Context->getSourceManager();
            SourceLocation Loc = Func->getLocation();

            // Get the line number of the function name
            unsigned funcLine = SM.getSpellingLineNumber(Loc);

            // CHECK: Is this line inside a // START ... // END block?
            if (Scanner.isInScope(funcLine)) {
                llvm::outs() << "Found function (In Scope): " << Func->getNameAsString() 
                             << " at line " << funcLine << "\n";
            } else {
                // Optional: Print ignored functions for debugging
                // llvm::outs() << "Ignored function (Out of Scope): " << Func->getNameAsString() << "\n";
            }
        }
    }
};

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    
    // 1. SCAN PHASE: Parse the first source file for comments
    // (In a real app, you would loop over all files)
    std::string sourcePath = OptionsParser.getSourcePathList()[0];
    ScopeScanner Scanner;
    Scanner.scanFile(sourcePath);

    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    // 2. MATCH PHASE: Pass the scanner to the printer
    FunctionPrinter Printer(Scanner);
    MatchFinder Finder;
    Finder.addMatcher(FunctionMatcher, &Printer);

    return Tool.run(newFrontendActionFactory(&Finder).get());
}