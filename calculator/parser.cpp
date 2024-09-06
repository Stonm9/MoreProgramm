#include "parser.h"

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto expression = ParseTerm(tokens, pos);
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<PlusToken>(token)) {
      expression = std::make_unique<Sum>(std::move(expression), ParseTerm(tokens, ++pos));  // sum A +B
    } else if (std::holds_alternative<MinusToken>(token)) {
      expression = std::make_unique<Subtract>(std::move(expression), ParseTerm(tokens, ++pos));  // sum A - B
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("Unknown symbol: " + std::get<UnknownToken>(token).value);
    } else {
      break;
    }
  }
  return expression;
}

std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto term = ParseFactor(tokens, pos);
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<MultiplyToken>(token)) {
      term = std::make_unique<Multiply>(std::move(term), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<ResidualToken>(token)) {
      term = std::make_unique<Residual>(std::move(term), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<DivideToken>(token)) {
      term = std::make_unique<Divide>(std::move(term), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("unknown symbol " + std::get<UnknownToken>(token).value);
    } else {
      break;
    }
  }
  return term;
}
std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos) {
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  const auto& token = tokens[pos++];
  if (std::holds_alternative<UnknownToken>(token)) {
    throw UnknownSymbolError("unknown token " + std::get<UnknownToken>(token).value);
  }
  if (std::holds_alternative<ClosingBracketToken>(token)) {
    throw WrongExpressionError("no matching (");
  }
  if (std::holds_alternative<OpeningBracketToken>(token)) {
    auto expression = ParseExpression(tokens, pos);
    if (pos >= tokens.size() || !std::holds_alternative<ClosingBracketToken>(tokens[pos++])) {
      throw WrongExpressionError("no matching )");
    }
    return expression;
  }
  if (std::holds_alternative<PlusToken>(token)) {
    return ParseFactor(tokens, pos);
  }
  if (std::holds_alternative<MinusToken>(token)) {
    auto arg = ParseFactor(tokens, pos);
    return std::make_unique<Minus>(std::move(arg));
  }
  if (std::holds_alternative<NumberToken>(token)) {
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }
  if (std::holds_alternative<AbsToken>(token)) {
    return ParseFactor(tokens, pos);
  }
  if (std::holds_alternative<SqrToken>(token)) {
    return ParseFactor(tokens, pos);
  }

  throw WrongExpressionError("invalid token");
}