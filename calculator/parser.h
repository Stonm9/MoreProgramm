#ifndef PARSER_H
#define PARSER_H

#include "tokenize/tokenize.h"
#include "polish_notation/expressions.h"
#include <string>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error {
 public:
  explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
  }
};

class WrongExpressionError : public std::runtime_error {
 public:
  explicit WrongExpressionError(const std::string& message = "")
      : std::runtime_error("WrongExpressionError: " + message) {
  }
};

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);
std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos);
std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos);

#endif