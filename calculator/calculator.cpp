#include "tokenize/tokenize.h"
#include "parser.h"

int CalculateExpression(std::string_view input) {
  const auto tokens = Tokenize(input);
  size_t pos = 0;
  const auto expression = ParseExpression(tokens, pos);
  if (pos < tokens.size()) {
    throw WrongExpressionError("extra tokens detected");
  }
  return expression->Calculate();
}