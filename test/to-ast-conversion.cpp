#include <fstream>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

#include "../include/ParserResult.hpp"

ParserResult parse(const char* inputFileName);

void compareFiles(const char* resultFileName, const char* expectedFileName) {
  std::ifstream result(resultFileName);
  std::ifstream expected(expectedFileName);

  std::stringstream resultStream;
  std::stringstream expectedStream;
  resultStream << result.rdbuf();
  expectedStream << expected.rdbuf();

  ASSERT_EQ(resultStream.str(), expectedStream.str());
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
