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

#ifndef BIO_BEDGRAPH_BEDGRAPH_H_
#define BIO_BEDGRAPH_BEDGRAPH_H_

#include <cstdint>
#include <string>

#include "absl/strings/str_format.h"

namespace bio {

// Represents an entry in a BedGraph file.
struct BedGraphEntry {
  // The chromosome name.
  std::string chromosome;

  // Feature start position.
  uint64_t start;

  // Feature end position.
  uint64_t end;

  // Track data value.
  double value;

  // Serializes the BedGraphEntry to its string representation.
  auto string() const -> std::string {
    return absl::StrFormat("%s\t%d\t%d\t%.2f", chromosome, start, end, value);
  }
};

}  // namespace bio

#endif  // BIO_BEDGRAPH_BEDGRAPH_H_
