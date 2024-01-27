#include <fstream>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

std::vector<std::string> extractResult(std::ifstream inputFile) {
  std::vector<std::string> result;

  std::string word;
  while (inputFile >> word) {
	if (word == ">") {
	  if (inputFile >> word) {
		result.push_back(word);
	  } else {
		break;
	  }
	}
  }

  return result;
}

void compileRunAndCheck(const std::string &directory,
						const std::string &program,
						const std::string &maszyna,
						std::vector<std::string> inputs) {
  for (const auto &input : inputs) {
	std::stringstream compileCommand;
	compileCommand << "../compiler " << directory << "/" << program << ".imp " << directory << "/" << program
				   << ".rm";

	std::stringstream runCommand;
	runCommand << "../virtual-machine/" << maszyna << " " << directory << "/" << program
			   << ".rm >" << directory << "/" << program << ".result < " << directory << "/" << program << "-in"
			   << input << ".txt";

	std::cout << compileCommand.str() << std::endl;
	std::cout << runCommand.str() << std::endl;

	system(compileCommand.str().c_str());
	system(runCommand.str().c_str());

	std::vector<std::string> result =
		extractResult(std::ifstream(directory + "/" + program + ".result"));
	std::vector<std::string> expected =
		extractResult(std::ifstream(directory + "/" + program + "-out" + input + ".txt"));

	ASSERT_EQ(result.size(), expected.size());
	for (size_t i = 0; i < result.size(); i++) {
	  ASSERT_EQ(result[i], expected[i]);
	}
  }
}

TEST(Compiler, Test1) {
  compileRunAndCheck("correct-programs", "example1", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test2) {
  compileRunAndCheck("correct-programs", "example2", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test3) {
  compileRunAndCheck("correct-programs", "example3", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test4) {
  compileRunAndCheck("correct-programs", "example4", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test5) {
  compileRunAndCheck("correct-programs", "example5", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test6) {
  compileRunAndCheck("correct-programs", "example6", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test7) {
  compileRunAndCheck("correct-programs", "example7", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test8) {
  compileRunAndCheck("correct-programs", "example8", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test9) {
  compileRunAndCheck("correct-programs", "example9", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test10) {
  compileRunAndCheck("translation-examples", "program0", "maszyna-wirtualna-cln", {"1", "2", "3"});
}

TEST(Compiler, Test11) {
  compileRunAndCheck("translation-examples", "program1", "maszyna-wirtualna-cln", {"1", "2", "3"});
}

TEST(Compiler, Test12) {
  compileRunAndCheck("translation-examples", "program2", "maszyna-wirtualna-cln", {""});
}

TEST(Compiler, Test13) {
  compileRunAndCheck("translation-examples", "program3", "maszyna-wirtualna-cln", {"1", "2", "3"});
}