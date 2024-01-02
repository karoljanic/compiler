#include <iostream>

#include "../include/compiler.hpp"

Compiler::Compiler() { }

void Compiler::compile(const char* filename) {
  std::cout << "Compiling " << filename << std::endl;
}