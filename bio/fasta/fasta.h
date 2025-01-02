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

#ifndef BIO_FASTA_FASTA_H_
#define BIO_FASTA_FASTA_H_

#include <cstdlib>
#include <string>

#include "absl/strings/str_format.h"
#include "bio/common/strings.h"

namespace bio {

// The number of characters per line when serializing a FASTA sequence to a
// string.
static constexpr size_t kCharsPerLine = 60;

// Contains data from a FASTA file for a single sequence.
struct FastaSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }

  // Serializes the sequence to its string format.
  auto string() const -> std::string {
    std::string seq = InsertWordEveryNthPosition(sequence, "\n", kCharsPerLine);
    return absl::StrFormat(">%s\n%s\n", name, seq);
  }
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_H_
