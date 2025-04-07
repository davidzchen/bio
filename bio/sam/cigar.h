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

#ifndef BIO_SAM_CIGAR_H_
#define BIO_SAM_CIGAR_H_

#include <cstdlib>
#include <string>
#include <vector>

namespace bio {

// Represents a CIGAR operation type.
enum class CigarType {
  kAlignmentMatch,    // M: Alignment match
  kInsertion,         // I: Insertion into the reference
  kDeletion,          // D: Deletion from the reference
  kSkippedRegion,     // N: Skipped region from the reference
  kSoftClipping,      // S: Soft clipping
  kHardClipping,      // H: Hard clipping
  kPadding,           // P: Padding
  kSequenceMatch,     // =: Sequence match
  kSequenceMismatch,  // X: Sequence mismatch
  kInvalid
};

// Represents a single CIGAR operation.
struct CigarOperation {
  // The cigar operation type.
  CigarType type;

  // The length of the operation.
  size_t length;

  // Checks for equality.
  auto operator==(const CigarOperation& rhs) const -> bool {
    return type == rhs.type && length == rhs.length;
  }

  // Serializes the CigarOperation into a string.
  auto string() const -> std::string;
};

// Represents a CIGAR string.
struct Cigar {
  std::vector<CigarOperation> operations;

  // Checks for equality.
  auto operator==(const Cigar& rhs) const -> bool;

  // Serializes the CigarString into a string.
  auto string() const -> std::string;
};

}  // namespace bio

#endif  // BIO_SAM_CIGAR_H_
