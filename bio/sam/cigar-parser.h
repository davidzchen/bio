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

#ifndef BIO_SAM_CIGAR_PARSER_H_
#define BIO_SAM_CIGAR_PARSER_H_

#include <string>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/sam/cigar.h"

namespace bio {

// Parser for CIGAR strings.
class CigarParser {
 public:
  CigarParser() = default;
  ~CigarParser() = default;

  // Parses the CIGAR string into a vector of operations.
  auto Parse(absl::string_view cigar) -> absl::StatusOr<Cigar>;
};

// Parses the CIGAR string or kills the program if parsing fails.
auto ParseCigarOrDie(absl::string_view cigar) -> Cigar;

}  // namespace bio

#endif  // BIO_SAM_CIGAR_PARSER_H_
