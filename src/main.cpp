#include <iostream>

#include "../include/compiler.hpp"

int compile(const char* in_file, const char* out_file);

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <source file> <output file>" << std::endl;
    return 1;
  }

  return compile(argv[1], argv[2]);

  //  Compiler compiler;
  //  compiler.compile(argv[1]);

  return 0;
}
