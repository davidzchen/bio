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

#include "bio/sam/cigar.h"

#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"

namespace bio {
namespace {

auto CigarTypeChar(CigarType type) -> char {
  switch (type) {
    case CigarType::kAlignmentMatch:
      return 'M';
    case CigarType::kInsertion:
      return 'I';
    case CigarType::kDeletion:
      return 'D';
    case CigarType::kSkippedRegion:
      return 'N';
    case CigarType::kSoftClipping:
      return 'S';
    case CigarType::kHardClipping:
      return 'H';
    case CigarType::kPadding:
      return 'P';
    case CigarType::kSequenceMatch:
      return '=';
    case CigarType::kSequenceMismatch:
      return 'X';
    default:  // case CigarType::kInvalid:
      return '#';
  }
}

}  // namespace

auto CigarOperation::string() const -> std::string {
  return absl::StrFormat("%d%c", length, CigarTypeChar(type));
}

auto Cigar::string() const -> std::string {
  std::string output;
  for (const auto& operation : operations) {
    absl::StrAppend(&output, operation.string());
  }
  return output;
}

}  // namespace bio
