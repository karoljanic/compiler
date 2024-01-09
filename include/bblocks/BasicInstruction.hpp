#ifndef BASIC_INSTRUCTION_HPP
#define BASIC_INSTRUCTION_HPP

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "../core/Hardware.hpp"
#include "../core/Utils.hpp"

class BasicInstruction {
 protected:
  std::shared_ptr<Hardware> hardware;

 public:
  BasicInstruction();
  BasicInstruction(std::shared_ptr<Hardware> hardware);
  virtual ~BasicInstruction() = default;
  BasicInstruction(const BasicInstruction& from) = default;
  BasicInstruction& operator=(const BasicInstruction& from) = default;
  BasicInstruction(BasicInstruction&&) = default;
  BasicInstruction& operator=(BasicInstruction&&) = default;

  virtual std::vector<std::pair<HardwareInstruction, std::string>> expandToHardwareInstructions() const;
  virtual void print(std::ostream& out) const;
};

#endif  // BASIC_INSTRUCTION_HPP
