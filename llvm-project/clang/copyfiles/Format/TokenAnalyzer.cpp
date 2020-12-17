//===--- TokenAnalyzer.cpp - Analyze Token Streams --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements an abstract TokenAnalyzer and associated helper
/// classes. TokenAnalyzer can be extended to generate replacements based on
/// an annotated and pre-processed token stream.
///
//===----------------------------------------------------------------------===//
#include "unistd.h"
#include <sys/stat.h>
#include <fstream>
#include "TokenAnalyzer.h"
#include "AffectedRangeManager.h"
#include "Encoding.h"
#include "FormatToken.h"
#include "FormatTokenLexer.h"
#include "TokenAnnotator.h"
#include "UnwrappedLineParser.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Format/Format.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "format-formatter"

namespace clang {
namespace format {

Environment::Environment(StringRef Code, StringRef FileName,
                         ArrayRef<tooling::Range> Ranges,
                         unsigned FirstStartColumn, unsigned NextStartColumn,
                         unsigned LastStartColumn):FileName(FileName),VirtualSM(new SourceManagerForFile(FileName, Code)), SM(VirtualSM->get()),
      ID(VirtualSM->get().getMainFileID()), FirstStartColumn(FirstStartColumn),
      NextStartColumn(NextStartColumn), LastStartColumn(LastStartColumn) {
  SourceLocation StartOfFile = SM.getLocForStartOfFile(ID);
  for (const tooling::Range &Range : Ranges) {
    SourceLocation Start = StartOfFile.getLocWithOffset(Range.getOffset());
    SourceLocation End = Start.getLocWithOffset(Range.getLength());
    CharRanges.push_back(CharSourceRange::getCharRange(Start, End));
  }
}

TokenAnalyzer::TokenAnalyzer(const Environment &Env, const FormatStyle &Style)
    : Style(Style), Env(Env),
      AffectedRangeMgr(Env.getSourceManager(), Env.getCharRanges()),
      UnwrappedLines(1),
      Encoding(encoding::detectEncoding(
          Env.getSourceManager().getBufferData(Env.getFileID()))) {
  LLVM_DEBUG(
      llvm::dbgs() << "File encoding: "
                   << (Encoding == encoding::Encoding_UTF8 ? "UTF8" : "unknown")
                   << "\n");
  LLVM_DEBUG(llvm::dbgs() << "Language: " << getLanguageName(Style.Language)
                          << "\n");
}

std::pair<tooling::Replacements, unsigned> TokenAnalyzer::process() {
  tooling::Replacements Result;
  llvm::SpecificBumpPtrAllocator<FormatToken> Allocator;
  IdentifierTable IdentTable(getFormattingLangOpts(Style));
  FormatTokenLexer Lex(Env.getSourceManager(), Env.getFileID(),
                       Env.getFirstStartColumn(), Style, Encoding, Allocator,
                       IdentTable);
  ArrayRef<FormatToken *> Toks(Lex.lex());
  SmallVector<FormatToken *, 10> Tokens(Toks.begin(), Toks.end());
  std::cout << "--------------------" <<std::endl;
  UnwrappedLineParser Parser(Style, Lex.getKeywords(),
                             Env.getFirstStartColumn(),Encoding, Tokens, *this , &(Env.getSourceManager()));
  Parser.parse();
  assert(UnwrappedLines.rbegin()->empty());
  mkdir("./files", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir("./files/temp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  std::ofstream inputs ("./files/temp/tempInputs");
  std::cout << "----" <<Parser.listInputs.size()<< "----";
  for(auto it = Parser.listInputs.begin(); it != Parser.listInputs.end();it++)
  {
	  assert(it->size() == 2);
	  assert("should check that the INPUT macro is in function main");
	  inputs << *(it->begin()) << "-" << *(++(it->begin())) << "$";
	  
  }
  std::string file_name = Env.FileName.str();
  std::string fileName = "./files/" + file_name.substr(file_name.find_last_of("\\/")+1,file_name.length()) ;
	std::ofstream newCode (fileName);
	std::cout << fileName;
	std::cout <<"###########OUTPUT############"<<UnwrappedLines.begin()->size()<<std::endl;
	newCode << "#include \"libFiles/gStatistics.h\"" << std::endl;
	std::cout << "#include \"libFiles/gStatistics.h\""<< std::endl;
 
    for (SmallVectorImpl<UnwrappedLine>::iterator I = UnwrappedLines.begin()->begin(), E = UnwrappedLines.begin()->end();
         I != E; ++I) {
		for (std::list<UnwrappedLineNode>::iterator it = I->Tokens.begin() ; it != I->Tokens.end(); ++it){
			if((it->Tok)->TokenText.str() == "_main_original" ){
				assert(  ((++it)->Tok)->TokenText.str() == "(");
				++it;
				std::cout << " _main_original(";
				newCode << " _main_original(";
				for(auto iter = Parser.listInputs.begin(); iter != Parser.listInputs.end();iter++){
					std::string type(*(iter->begin())); 
					std::string var(*(++(iter->begin())));
					if(iter == Parser.listInputs.begin()){
						newCode <<type << " " << var ;
						std::cout <<type << " " << var;
					}
					else{
						newCode <<","<<type << " " << var ;
						std::cout <<","<<type << " " << var;
						}
					
			}
		}
		std::cout << whiteSpaceAdding(Env.getSourceManager(),(it->Tok)->WhitespaceRange)<<(it->Tok)->TokenText.str();
		newCode << whiteSpaceAdding(Env.getSourceManager(),(it->Tok)->WhitespaceRange)<<(it->Tok)->TokenText.str();
	}

    }
    newCode.flush();
	std::cout <<"###########OUTPUT############"<<std::endl;
	newCode.close();
  /*unsigned Penalty = 0;
  for (unsigned Run = 0, RunE = UnwrappedLines.size(); Run + 1 != RunE; ++Run) {
    LLVM_DEBUG(llvm::dbgs() << "Run " << Run << "...\n");
    SmallVector<AnnotatedLine *, 16> AnnotatedLines;

    TokenAnnotator Annotator(Style, Lex.getKeywords());
    for (unsigned i = 0, e = UnwrappedLines[Run].size(); i != e; ++i) {
      AnnotatedLines.push_back(new AnnotatedLine(UnwrappedLines[Run][i]));
      Annotator.annotate(*AnnotatedLines.back());
    }

    std::pair<tooling::Replacements, unsigned> RunResult =
        analyze(Annotator, AnnotatedLines, Lex);

    LLVM_DEBUG({
      llvm::dbgs() << "Replacements for run " << Run << ":\n";
      for (tooling::Replacements::const_iterator I = RunResult.first.begin(),
                                                 E = RunResult.first.end();
           I != E; ++I) {
        llvm::dbgs() << I->toString() << "\n";
      }
    });
    for (unsigned i = 0, e = AnnotatedLines.size(); i != e; ++i) {
      delete AnnotatedLines[i];
    }

    Penalty += RunResult.second;
    for (const auto &R : RunResult.first) {
      auto Err = Result.add(R);
      // FIXME: better error handling here. For now, simply return an empty
      // Replacements to indicate failure.
      if (Err) {
        llvm::errs() << llvm::toString(std::move(Err)) << "\n";
        return {tooling::Replacements(), 0};
      }
    }
  }*/
  return {Result, /*Penalty*/0};
}

void TokenAnalyzer::consumeUnwrappedLine(const UnwrappedLine &TheLine) {
  assert(!UnwrappedLines.empty());
  UnwrappedLines.back().push_back(TheLine);
}

void TokenAnalyzer::finishRun() {
  UnwrappedLines.push_back(SmallVector<UnwrappedLine, 16>());
}


} // end namespace format
} // end namespace clang
