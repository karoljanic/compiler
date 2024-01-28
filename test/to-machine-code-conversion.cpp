#include <fstream>
#include <iostream>
#include <sstream>
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

void compileRunAndCheck(const std::string& program, const std::string& maszyna, std::vector<std::string> inputs = {""}) {
  for (const auto &input : inputs) {
	std::stringstream compileCommand;
	compileCommand << "../compiler machine-code-programs/" << program << ".imp machine-code-programs/" << program
				   << ".rm";

	std::stringstream runCommand;
	runCommand << "../virtual-machine/" << maszyna << " machine-code-programs/" << program
			   << ".rm > machine-code-programs/"
			   << program << ".result < machine-code-programs/" << program << "-in" << input << ".txt";

	std::cout << compileCommand.str() << std::endl;
	std::cout << runCommand.str() << std::endl;

	system(compileCommand.str().c_str());
	system(runCommand.str().c_str());

	std::vector<std::string> result =
		extractResult(std::ifstream("machine-code-programs/" + program + ".result"));
	std::vector<std::string> expected =
		extractResult(std::ifstream("machine-code-programs/" + program + "-out" + input + ".txt"));

	ASSERT_EQ(result.size(), expected.size());
	for (size_t i = 0; i < result.size(); i++) {
	  ASSERT_EQ(result[i], expected[i]);
	}
  }
}

TEST(toMachineCodeConversion, readWrite) {
#ifdef CLN
  compileRunAndCheck("io", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("io", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, assignment) {
#ifdef CLN
compileRunAndCheck("assigns", "maszyna-wirtualna-cln");
#else
compileRunAndCheck("assigns", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, registersAllocation) {
#ifdef CLN
  compileRunAndCheck("reg-alloc", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("reg-alloc", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, addition) {
#ifdef CLN
  compileRunAndCheck("adds", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("adds", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, subtraction) {
#ifdef CLN
  compileRunAndCheck("subs", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("subs", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, multiplication) {
#ifdef CLN
  compileRunAndCheck("muls", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("muls", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, division) {
#ifdef CLN
  compileRunAndCheck("divs", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("divs", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, modulo) {
#ifdef CLN
  compileRunAndCheck("mods", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("mods", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, ifElse) {
#ifdef CLN
  compileRunAndCheck("if", "maszyna-wirtualna-cln", {"1", "2", "3", "4"});
#else
  compileRunAndCheck("if", "maszyna-wirtualna", {"1", "2", "3", "4"});
#endif
}

TEST(toMachineCodeConversion, ifElse2) {
#ifdef CLN
  compileRunAndCheck("if2", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("if2", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, ifElse3) {
#ifdef CLN
  compileRunAndCheck("if3", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("if3", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, whileLoop) {
#ifdef CLN
  compileRunAndCheck("while", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("while", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, repeatLoop) {
#ifdef CLN
  compileRunAndCheck("repeat", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("repeat", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, functionCall1) {
#ifdef CLN
  compileRunAndCheck("fun1", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("fun1", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, functionCall2) {
#ifdef CLN
  compileRunAndCheck("fun2", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("fun2", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, functionCall3) {
#ifdef CLN
  compileRunAndCheck("fun3", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("fun3", "maszyna-wirtualna");
#endif
}

TEST(toMachineCodeConversion, functionCall4) {
#ifdef CLN
  compileRunAndCheck("fun4", "maszyna-wirtualna-cln");
#else
  compileRunAndCheck("fun4", "maszyna-wirtualna");
#endif
}