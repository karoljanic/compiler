#include <fstream>
#include <iostream>

#include "../include/core/Compiler.hpp"
#include "../include/core/ParserResult.hpp"

ParserResult parse(const char* inputFileName);

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <source file> <output file>" << std::endl;
    return 1;
  }

  std::ofstream outputFile(argv[2]);
  if (!outputFile) {
    std::cerr << "Error: could not open/create output file " << argv[2] << std::endl;
    return 1;
  }

  const auto result = parse(argv[1]);
  if (result.result == ParserResultCode::SUCCESS) {
    std::ofstream astFile("ast.txt");
    std::ofstream bb1File("bb1.txt");
    std::ofstream bb2File("bb2.txt");

    Compiler compiler(result.ast);
    compiler.generateMachineCodeWithDebug(astFile, bb1File, bb2File, outputFile);
  }

  return 0;
}
