#include <gtest/gtest.h>

void runTest(std::string testCase, std::string testName) {
  ::testing::GTEST_FLAG(filter) = std::string(testCase + "." + testName);
  int result = RUN_ALL_TESTS();

  if (result != 0) {
    std::cerr << "Test " << testCase << " failed with result code: " << result << std::endl;
  }
}

int main() {
  ::testing::InitGoogleTest();

  const ::testing::UnitTest& unitTest = *::testing::UnitTest::GetInstance();
  const size_t numTestCases = unitTest.total_test_case_count();

  for (size_t i = 0; i < numTestCases; ++i) {
    const ::testing::TestCase* testCase = unitTest.GetTestCase(i);
    const size_t numTests = testCase->total_test_count();

    for (size_t j = 0; j < numTests; ++j) {
      const ::testing::TestInfo* testInfo = testCase->GetTestInfo(j);

      pid_t pid = fork();

      if (pid == 0) {  // Child process
        runTest(testCase->name(), testInfo->name());
        exit(0);
      }
      else if (pid > 0) {  // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
          std::cerr << "Child process failed for test case: " << testCase->name() << "." << testInfo->name() << std::endl;
          return 1;
        }
      }
      else {
        std::cerr << "Error creating child process for test case: " << testCase->name() << "." << testInfo->name() << std::endl;
        return 1;
      }
    }
  }

  return 0;
}
