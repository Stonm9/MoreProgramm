#ifndef TOKENIZE_TOKENIZE_H
#define TOKENIZE_TOKENIZE_H

#include <vector>
#include <string_view>
#include <variant>
#include <string>
struct PlusToken {};
inline bool operator==(PlusToken, PlusToken) {
  return true;
}
struct MinusToken {};
inline bool operator==(MinusToken, MinusToken) {
  return true;
}
struct MultiplyToken {};  // Умножение
inline bool operator==(MultiplyToken, MultiplyToken) {
  return true;
}
struct DivideToken {};  // Деление
inline bool operator==(DivideToken, DivideToken) {
  return true;
}
struct ResidualToken {};  // Взятие остатка
inline bool operator==(ResidualToken, ResidualToken) {
  return true;
}
struct OpeningBracketToken {};
inline bool operator==(OpeningBracketToken, OpeningBracketToken) {
  return true;
}
struct ClosingBracketToken {};
inline bool operator==(ClosingBracketToken, ClosingBracketToken) {
  return true;
}
struct MinToken {};
inline bool operator==(MinToken, MinToken) {
  return true;
}
struct MaxToken {};
inline bool operator==(MaxToken, MaxToken) {
  return true;
}
struct AbsToken {};
inline bool operator==(AbsToken, AbsToken) {
  return true;
}
struct SqrToken {};
inline bool operator==(SqrToken, SqrToken) {
  return true;
}
struct NumberToken {
  int value;
};
struct UnknownToken {
  std::string value;
};
inline bool operator==(const NumberToken& lhs, const NumberToken& rhs) {
  return lhs.value == rhs.value;
}
inline bool operator==(const UnknownToken& lhs, const UnknownToken& rhs) {
  return lhs.value == rhs.value;
}
// Псевданим
using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                           ClosingBracketToken, MinToken, MaxToken, AbsToken, SqrToken, NumberToken, UnknownToken>;

std::vector<Token> Tokenize(std::string_view input);

#endif