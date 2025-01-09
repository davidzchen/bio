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

  // Optional columns.
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

  // Returns the string (on-file) representation of the MRF header.
  auto string() const -> std::string;

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
  // One or more blocks comprising the read.
  std::vector<MrfBlock> blocks;

  // The read's sequence.
  std::string sequence;

  // The read's quality scores.
  std::string quality_scores;

  // The read's query ID.
  std::string query_id;

  // Computes the read length of this read.
  auto Length() -> size_t;
};

// Represents an entry in an MRF file.
struct MrfEntry {
  // Whether the entry contains a paired end read.
  bool is_paired_end;

  // The first read. If this entry is not a paied -end read, then only this
  // field will be populated.
  MrfRead read1;

  // The second read. This is populated only if this entry is a paired end read.
  MrfRead read2;

  // Validates the entry against the header.
  auto Validate(const MrfHeader& header) -> absl::Status;
};

}  // namespace bio

#endif  // BIO_MRF_MRF_H_
