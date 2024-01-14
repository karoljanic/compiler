#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#undef CLN

std::vector<std::string> extractResult(std::ifstream inputFile) {
  std::vector<std::string> result;

  std::string word;
  while (inputFile >> word) {
    if (word == ">") {
      if (inputFile >> word) {
        result.push_back(word);
      }
      else {
        break;
      }
    }
  }

  return result;
}

void compileRunAndCheck(const std::string& program, const std::string& maszyna) {
  std::stringstream compileCommand;
  compileCommand << "../compiler machine-code-programs/" << program << ".imp machine-code-programs/" << program << ".rm";

  std::stringstream runCommand;
  runCommand << "../virtual-machine/" << maszyna << " machine-code-programs/" << program << ".rm > machine-code-programs/"
             << program << ".result < machine-code-programs/" << program << "-in.txt";

  std::cout << compileCommand.str() << std::endl;
  std::cout << runCommand.str() << std::endl;

  system(compileCommand.str().c_str());
  system(runCommand.str().c_str());

  std::vector<std::string> result = extractResult(std::ifstream("machine-code-programs/" + program + ".result"));
  std::vector<std::string> expected = extractResult(std::ifstream("machine-code-programs/" + program + "-out.txt"));

  ASSERT_EQ(result.size(), expected.size());
  for (size_t i = 0; i < result.size(); i++) {
    ASSERT_EQ(result[i], expected[i]);
  }
}

TEST(toMachineCodeConversion, readWrite) {
#ifdef CLN
  compileRunAndCheck("io", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("io", "maszyna-wirtualna");
#endif
}