#include <fstream>

#include "gtest/gtest.h"

int parse(const char* inputFileName);

TEST(toAstConversion, valuesCreating) {
  compile("grammar/values-creating.imp");
}
