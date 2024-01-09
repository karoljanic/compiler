#include <fstream>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

#include "../include/core/ParserResult.hpp"

ParserResult parse(const char* inputFileName);

void compareFiles(const char* resultFileName, const char* expectedFileName) {
  std::ifstream result(resultFileName);
  std::ifstream expected(expectedFileName);

  std::stringstream resultStream;
  std::stringstream expectedStream;
  resultStream << result.rdbuf();
  expectedStream << expected.rdbuf();

  ASSERT_EQ(resultStream.str(), expectedStream.str());

  result.close();
  expected.close();
}

TEST(toAstConversion, inputFileNotFound) {
  const auto result = parse("file-not-exists.imp");

  ASSERT_EQ(result.result, ParserResultCode::FILE_NOT_FOUND);
  ASSERT_EQ(result.ast, nullptr);
}

TEST(toAstConversion, example1) {
  const auto parseResult = parse("correct-programs/example1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example1.ast");
}

TEST(toAstConversion, example2) {
  const auto parseResult = parse("correct-programs/example2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example2.ast");
}

TEST(toAstConversion, example3) {
  const auto parseResult = parse("correct-programs/example3.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example3.ast");
}

TEST(toAstConversion, example4) {
  const auto parseResult = parse("correct-programs/example4.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example4.ast");
}

TEST(toAstConversion, example5) {
  const auto parseResult = parse("correct-programs/example5.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example5.ast");
}

TEST(toAstConversion, example6) {
  const auto parseResult = parse("correct-programs/example6.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example6.ast");
}

TEST(toAstConversion, example7) {
  const auto parseResult = parse("correct-programs/example7.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example7.ast");
}

TEST(toAstConversion, example8) {
  const auto parseResult = parse("correct-programs/example8.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example8.ast");
}

TEST(toAstConversion, example9) {
  const auto parseResult = parse("correct-programs/example9.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SUCCESS);
  ASSERT_NE(parseResult.ast, nullptr);

  std::ofstream ast("tmp.ast");
  parseResult.ast->print(ast, 0);
  ast.close();

  compareFiles("tmp.ast", "correct-programs/example9.ast");
}

TEST(toAstConversion, error1) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error1.ast");
}

TEST(toAstConversion, error2) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error2.ast");
}

TEST(toAstConversion, error3) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error3.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error3.ast");
}

TEST(toAstConversion, error4) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error4.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error4.ast");
}

TEST(toAstConversion, error5) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error5.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error5.ast");
}

TEST(toAstConversion, error6) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error6.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error6.ast");
}

TEST(toAstConversion, error7) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error7.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error7.ast");
}

TEST(toAstConversion, error8) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("incorrect-programs/error8.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "incorrect-programs/error8.ast");
}

TEST(toAstConversion, grammarCheck1) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/read.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/read.ast");
}

TEST(toAstConversion, grammarCheck2) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/write1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/write1.ast");
}

TEST(toAstConversion, grammarCheck3) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/write2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/write2.ast");
}

TEST(toAstConversion, grammarCheck4) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations1.ast");
}

TEST(toAstConversion, grammarCheck5) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations2.ast");
}

TEST(toAstConversion, grammarCheck6) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations3.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations3.ast");
}

TEST(toAstConversion, grammarCheck7) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations4.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations4.ast");
}

TEST(toAstConversion, grammarCheck8) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations5.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations5.ast");
}

TEST(toAstConversion, grammarCheck9) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations6.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations6.ast");
}

TEST(toAstConversion, grammarCheck10) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/declarations7.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/declarations7.ast");
}

TEST(toAstConversion, grammarCheck11) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment1.ast");
}

TEST(toAstConversion, grammarCheck12) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment2.ast");
}

TEST(toAstConversion, grammarCheck13) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment3.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment3.ast");
}

TEST(toAstConversion, grammarCheck14) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment4.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment4.ast");
}

TEST(toAstConversion, grammarCheck15) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment5.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment5.ast");
}

TEST(toAstConversion, grammarCheck16) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/assignment6.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/assignment6.ast");
}

TEST(toAstConversion, grammarCheck17) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/condition1.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/condition1.ast");
}

TEST(toAstConversion, grammarCheck18) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/condition2.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/condition2.ast");
}

TEST(toAstConversion, grammarCheck19) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/condition3.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/condition3.ast");
}

TEST(toAstConversion, grammarCheck20) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/condition4.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SEMANTIC_ERROR);

  compareFiles("tmp.ast", "grammar-components/condition4.ast");
}

TEST(toAstConversion, grammarCheck21) {
  freopen("tmp.ast", "w", stderr);
  const auto parseResult = parse("grammar-components/condition5.imp");

  ASSERT_EQ(parseResult.result, ParserResultCode::SYNTAX_ERROR);

  compareFiles("tmp.ast", "grammar-components/condition5.ast");
}
