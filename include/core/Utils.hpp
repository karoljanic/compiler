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

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarVarMultiplication(
      const std::string& var1, const std::string& var2, const std::string& result, std::shared_ptr<Hardware> hardware);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarNumberAddition(const std::string& var,
                                                                                            uint64_t number,
                                                                                            const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarNumberSubtraction(
      const std::string& var, uint64_t number, const std::string& result, std::shared_ptr<Hardware> hardware);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateNumberVarSubtraction(uint64_t number,
                                                                                               const std::string& var,

                                                                                               const std::string& result);

  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarNumberMultiplication(const std::string& var,
                                                                                                  uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarVarDivision(const std::string& var1,
                                                                                         const std::string& var2);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarNumberDivision(const std::string& var,
                                                                                            uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateNumberVarDivision(const std::string& var,
                                                                                            uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarVarModulo(const std::string& var1,
                                                                                       const std::string& var2);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateVarNumberModulo(const std::string& var,
                                                                                          uint64_t number);
  static std::vector<std::pair<HardwareInstruction, std::string>> generateNumberVarModulo(const std::string& var,
                                                                                          uint64_t number);
};

#endif  // UTILS_HPP