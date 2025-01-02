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

#ifndef BIO_COMMON_STRINGS_H_
#define BIO_COMMON_STRINGS_H_

#include <cstdlib>
#include <string>

#include "absl/strings/string_view.h"

namespace bio {

// Returns the first word in the string.
auto FirstWord(absl::string_view str) -> std::string;

// Inserts the given word at every nth position in the string, but not at the
// end.
auto InsertWordEveryNthPosition(absl::string_view str, absl::string_view word,
                                size_t n) -> std::string;

}  // namespace bio

#endif  // BIO_COMMON_STRINGS_H_
