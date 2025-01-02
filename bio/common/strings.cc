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

#include "bio/common/strings.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace bio {

auto FirstWord(absl::string_view str) -> std::string {
  absl::string_view trimmed = absl::StripLeadingAsciiWhitespace(str);
  const std::vector<std::string> parts = absl::StrSplit(trimmed, " ");
  return parts[0];
}

auto InsertWordEveryNthPosition(absl::string_view str, absl::string_view word,
                                size_t n) -> std::string {
  std::string buffer;
  int i = 0;
  while (i < str.size()) {
    absl::StrAppend(&buffer, absl::string_view(&str[i], 1));
    ++i;
    if ((i % n) == 0 && i != str.size()) {
      absl::StrAppend(&buffer, word);
    }
  }
  return buffer;
}

}  // namespace bio
