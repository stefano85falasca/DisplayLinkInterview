#pragma once
#include <algorithm>

template <class TContainer>
bool begins_with(const TContainer &match, const TContainer &input) {
  return input.size() >= match.size() &&
         std::equal(match.begin(), match.end(), input.begin());
}
