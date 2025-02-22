// Copyright 2017 The Bio Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bio/common/word-iterator.h"

#include <cstdlib>
#include <optional>
#include <string>

#include "absl/strings/string_view.h"

namespace bio {

WordIterator::WordIterator(absl::string_view str, absl::string_view separators)
    : str_(std::string(str)), start_cursor_(0), lookahead_cursor_(0) {
  for (char c : separators) {
    separators_.insert(c);
  }
}

auto WordIterator::Next() -> std::optional<absl::string_view> {
  if (str_.empty()) {
    return std::nullopt;
  }

  while (lookahead_cursor_ < str_.size()) {
    if (separators_.contains(str_[lookahead_cursor_])) {
      size_t len = lookahead_cursor_ - start_cursor_;
      size_t start = start_cursor_;
      ++lookahead_cursor_;
      start_cursor_ = lookahead_cursor_;
      return absl::string_view(&str_[start], len);
    }

    if (lookahead_cursor_ == str_.size() - 1) {
      ++lookahead_cursor_;
      int len = lookahead_cursor_ - start_cursor_;
      size_t start = start_cursor_;
      return absl::string_view(&str_[start], len);
    }
    ++lookahead_cursor_;
  }
  return std::nullopt;
}

auto WordIterator::eol() const -> bool {
  return lookahead_cursor_ == str_.size();
}

}  // namespace bio
