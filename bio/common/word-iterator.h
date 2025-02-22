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

#ifndef BIO_COMMON_WORD_ITERATOR_H_
#define BIO_COMMON_WORD_ITERATOR_H_

#include <cstdlib>
#include <optional>
#include <string>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/string_view.h"

namespace bio {

// Iterates over a string, returning words separated by the specified separator
// characters.
//
// Examples:
//
// ```
// WordIterator it("AA0BB1CC2DD", "012");
// EXPECT_THAT(it.Next(), ::testing::Optional("AA"));
// EXPECT_THAT(it.Next(), ::testing::Optional("BB"));
// EXPECT_THAT(it.Next(), ::testing::Optional("CC"));
// EXPECT_THAT(it.Next(), ::testing::Optional("DD"));
// EXPECT_FALSE(it.Next().has_value());
// ```
//
// ```
// WordIterator it("AA0BB12CC", "012");
// EXPECT_THAT(it.Next(), ::testing::Optional("AA"));
// EXPECT_THAT(it.Next(), ::testing::Optional("BB"));
// EXPECT_THAT(it.Next(), ::testing::Optional(""));
// EXPECT_THAT(it.Next(), ::testing::Optional("CC"));
// EXPECT_FALSE(it.Next().has_value());
// ```
class WordIterator {
 public:
  explicit WordIterator(absl::string_view str, absl::string_view separators);

  // Returns the next word.
  auto Next() -> std::optional<absl::string_view>;

 private:
  std::string str_;
  absl::flat_hash_set<char> separators_;
  size_t start_cursor_;
  size_t lookahead_cursor_;
};

}  // namespace bio

#endif  // BIO_COMMON_WORD_ITERATOR_H_
