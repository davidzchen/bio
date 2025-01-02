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

#ifndef BIO_FASTQ_FASTQ_H_
#define BIO_FASTQ_FASTQ_H_

#include <cstdlib>
#include <string>

#include "absl/strings/str_format.h"

namespace bio {

// Contains data from a FASTQ file for a single sequence.
struct FastqSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // The quality values for the sequence.
  std::string quality;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }

  // Serializes the sequence to its string format.
  auto string() const -> std::string {
    return absl::StrFormat("@%s\n%s\n+\n%s\n", name, sequence, quality);
  }
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_H_
