#include "string_view.h"
#include <utility>

void StringView::Swap(StringView& other) {
  std::swap(str_, other.str_);
  std::swap(end_, other.end_);
}