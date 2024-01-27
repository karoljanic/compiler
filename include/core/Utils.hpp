#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Hardware.hpp"

typedef std::pair<std::string, uint64_t> MachineCodeLabelType;
typedef std::pair<std::vector<MachineCodeLabelType>, std::pair<HardwareInstruction, std::string>> MachineCodeType;

class Utils {
 public:
  static bool additionIsSafe(uint64_t a, uint64_t b);

  static bool multiplicationIsSafe(uint64_t a, uint64_t b);

  static std::vector<bool> getReversedBits(uint64_t number);

  static std::vector<MachineCodeType> generateNumber(uint64_t number,
													 const std::string &registerName);

  static std::vector<MachineCodeType> generateVVAddition(const std::string &var1,
														 const std::string &var2,
														 const std::string &result);

  static std::vector<MachineCodeType> generateVNAddition(const std::string &var,
														 uint64_t number,
														 const std::string &result);

  static std::vector<MachineCodeType> generateNNAddition(uint64_t number1, uint64_t number2,
														 const std::string &result,
														 std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateVVSubtraction(const std::string &var1,
															const std::string &var2,
															const std::string &result);

  static std::vector<MachineCodeType> generateVNSubtraction(const std::string &var,
															uint64_t number,
															const std::string &result,
															std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateNVSubtraction(uint64_t number,
															const std::string &var,
															const std::string &result);

  static std::vector<MachineCodeType> generateNNSubtraction(uint64_t number1,
															uint64_t number2,
															const std::string &result);

  static std::vector<MachineCodeType> generateVVMultiplication(const std::string &var1,
															   const std::string &var2,
															   const std::string &result,
															   std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateVNMultiplication(const std::string &var,
															   uint64_t number,
															   const std::string &result);

  static std::vector<MachineCodeType> generateNNMultiplication(uint64_t number1,
															   uint64_t number2,
															   const std::string &result,
															   std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateVVDivision(const std::string &var1,
														 const std::string &var2,
														 const std::string &result,
														 std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateVNDivision(const std::string &var,
														 uint64_t number,
														 const std::string &result,
														 std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateNVDivision(const std::string &var,
														 uint64_t number,
														 const std::string &result,
														 std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateNNDivision(uint64_t number1,
														 uint64_t number2,
														 const std::string &result);

  static std::vector<MachineCodeType> generateVVModulo(const std::string &var1,
													   const std::string &var2,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateVNModulo(const std::string &var,
													   uint64_t number,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateNVModulo(const std::string &var,
													   uint64_t number,
													   const std::string &result,
													   std::shared_ptr<Hardware> hardware);

  static std::vector<MachineCodeType> generateNNModulo(uint64_t number1,
													   uint64_t number2,
													   const std::string &result);
};

#endif  // UTILS_HPP