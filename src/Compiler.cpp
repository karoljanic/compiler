#include <iostream>

#include "../include/Compiler.hpp"

Compiler::Compiler() {}

void Compiler::compile(const char* filename) {
  std::cout << "Compiling " << filename << std::endl;
}