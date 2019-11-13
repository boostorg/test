// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/Utils.h"

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

void
preprocess( const std::string& infile, const std::string& outf )
{
 clang::CompilerInstance inst;
 std::unique_ptr<raw_pwrite_stream> myos = inst.createOutputFile(
  outf,
  false,
  true,
  "",
  ".i",
  false);

 inst.createFileManager();
 inst.createDiagnostics();
 clang::TargetOptions& options = inst.getTargetOpts();
 options.Triple = "i486-ubuntu-linux";

 clang::TargetInfo* target = clang::TargetInfo::CreateTargetInfo(
      inst.getDiagnostics(),
      std::make_shared<clang::TargetOptions>(options)); // not sure
 inst.setTarget(target);
 inst.createSourceManager(inst.getFileManager());
 inst.InitializeSourceManager(clang::FrontendInputFile(infile, clang::InputKind(clang::InputKind::CXX)));
 clang::PreprocessorOptions& opts = inst.getPreprocessorOpts();
 //Push back all of the options...the defs can be “FOO=val”
 //opts.addMacroDef( … );
 inst.createPreprocessor(clang::TranslationUnitKind::TU_Complete);
  std::shared_ptr<clang::Preprocessor> preprocessor = inst.getPreprocessorPtr();
 clang::DoPrintPreprocessedInput(
  *preprocessor,
  &(*myos),
  inst.getPreprocessorOutputOpts() );

 //myos->close();
}

int main(int argc, const char **argv) {
  preprocess("/Users/raffi/code/boost/libs/test/include/boost/test/impl/framework.ipp", "/tmp/toto.cpp");

  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
#if 0
  return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
#endif
return 0;
}
