#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Hardware.hpp"

class Utils {
 public:
  static std::vector<std::pair<HardwareInstruction, std::string>> generateNumber(uint64_t number,
                                                                                 const std::string& registerName);

  static bool additionIsSafe(uint64_t a, uint64_t b);
  static bool multiplicationIsSafe(uint64_t a, uint64_t b);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVVAddition(const std::string& var1,
                                                                                     const std::string& var2,
                                                                                     const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVNAddition(const std::string& var, uint64_t number,
                                                                                     const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateNNAddition(uint64_t number1, uint64_t number2,
                                                                                     const std::string& result,
                                                                                     std::shared_ptr<Hardware> hardware);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVVSubtraction(const std::string& var1,
                                                                                        const std::string& var2,
                                                                                        const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVNSubtraction(const std::string& var, uint64_t number,
                                                                                        const std::string& result,
                                                                                        std::shared_ptr<Hardware> hardware);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateNVSubtraction(uint64_t number, const std::string& var,
                                                                                        const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateNNSubtraction(uint64_t number1, uint64_t number2,
                                                                                        const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVVMultiplication(const std::string& var1,
                                                                                           const std::string& var2,
                                                                                           const std::string& result,
                                                                                           std::shared_ptr<Hardware> hardware);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVNMultiplication(const std::string& var,
                                                                                           uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVVDivision(const std::string& var1,
                                                                                     const std::string& var2);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVNDivision(const std::string& var, uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateNVDivision(const std::string& var, uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVVModulo(const std::string& var1,
                                                                                   const std::string& var2);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVNModulo(const std::string& var, uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateNVModulo(const std::string& var, uint64_t number);
};

#endif  // UTILS_HPP