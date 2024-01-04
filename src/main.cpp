#include <fstream>
#include <iostream>

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
    result.ast->print(std::cout, 0);
  }

  return 0;
}
