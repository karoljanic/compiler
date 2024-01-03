#ifndef PARSER_RESULT_HPP
#define PARSER_RESULT_HPP

#include <memory>

#include "LookupTable.hpp"
#include "ast/AstProgram.hpp"

enum ParserResultCode { SUCCESS, FILE_NOT_FOUND, SYNTAX_ERROR };

struct ParserResult {
  ParserResultCode result;
  std::shared_ptr<AstProgram> ast;
  LookupTable* lookupTable;
};

#endif  // PARSER_RESULT_HPP
