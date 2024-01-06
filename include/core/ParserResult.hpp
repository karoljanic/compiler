#ifndef PARSER_RESULT_HPP
#define PARSER_RESULT_HPP

#include <memory>

#include "../ast/AstProgram.hpp"
#include "LookupTable.hpp"

enum ParserResultCode { SUCCESS, FILE_NOT_FOUND, SYNTAX_ERROR, SEMANTIC_ERROR };

struct ParserResult {
  ParserResultCode result;
  std::shared_ptr<AstProgram> ast;
};

#endif  // PARSER_RESULT_HPP
