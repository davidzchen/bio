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

#ifndef BIO_MRF_MRF_H_
#define BIO_MRF_MRF_H_

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/common/sequence.h"

namespace bio {

// Represents a Column in an MRF file.
enum class MrfColumn {
  // Required.
  kAlignmentBlocks,

  // Optional.
  kSequence,
  kQualityScores,
  kQueryId,
};

// Represents the header in an MRF file.
class MrfHeader {
 public:
  explicit MrfHeader(const std::vector<MrfColumn>& columns)
      : columns_(columns), present_columns_(columns.begin(), columns.end()) {}

  // Parses the provided line and returns an MrfHeader.
  static auto Parse(absl::string_view line)
      -> absl::StatusOr<std::unique_ptr<MrfHeader>>;

  // Adds comments that precedes the columns line.
  auto AddComments(const std::vector<std::string>& comments) -> void;

  // Returns a reference to the comments contained in the header.
  auto comments() const -> const std::vector<std::string>&;

  // Returns whether the specified columns is present.
  auto HasColumn(MrfColumn column) const -> bool;

  // Adds the specified column to the header. If the column is already contained
  // in the header, this call is a no-op.
  auto AddColumn(MrfColumn column) -> void;

  // Returns a reference to the columns in the header.
  auto columns() const -> const std::vector<MrfColumn>&;

 private:
  std::vector<MrfColumn> columns_;
  absl::flat_hash_set<MrfColumn> present_columns_;
  std::vector<std::string> comments_;
};

// Represents a block in an MRF read.
struct MrfBlock {
  std::string target_name;
  Strand strand;
  uint64_t target_start;
  uint64_t target_end;
  uint64_t query_start;
  uint64_t query_end;
};

// Represents an MRF read.
struct MrfRead {
  std::vector<MrfBlock> blocks;
  std::string sequence;
  std::string quality_scores;
  std::string query_id;

  // Computes the read length of this read.
  auto Length() -> size_t;
};

// Represents an entry in an MRF file.
struct MrfEntry {
  bool is_paired_end;
  MrfRead read1;
  MrfRead read2;
};

}  // namespace bio

#endif  // BIO_MRF_MRF_H_
