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

#include "bio/mrf/mrf.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

static constexpr char kColumnNameAlignmentBlocks[] = "AlignmentBlocks";
static constexpr char kColumnNameSequence[] = "Sequence";
static constexpr char kColumnNameQualityScores[] = "QualityScores";
static constexpr char kColumnNameQueryId[] = "QueryId";

}  // namespace

auto MrfHeader::Parse(absl::string_view line)
    -> absl::StatusOr<std::unique_ptr<MrfHeader>> {
  std::vector<std::string> parts = absl::StrSplit(line, "\t");
  std::vector<MrfColumn> columns;
  absl::flat_hash_set<MrfColumn> present_columns;
  for (const auto& part : parts) {
    MrfColumn column;
    if (part == kColumnNameAlignmentBlocks) {
      column = MrfColumn::kAlignmentBlocks;
    } else if (part == kColumnNameSequence) {
      column = MrfColumn::kSequence;
    } else if (part == kColumnNameQualityScores) {
      column = MrfColumn::kQualityScores;
    } else if (part == kColumnNameQueryId) {
      column = MrfColumn::kQueryId;
    } else {
      return absl::InvalidArgumentError(
          absl::StrFormat("Invalid column name: %s", part));
    }
    if (present_columns.contains(column)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Duplicate column %s found", part));
    }
    present_columns.insert(column);
    columns.push_back(column);
  }

  if (!present_columns.contains(MrfColumn::kAlignmentBlocks)) {
    return absl::InvalidArgumentError(
        "Required AlignmentBlocks column not present");
  }

  return std::make_unique<MrfHeader>(columns);
}

auto MrfHeader::AddComments(const std::vector<std::string>& comments) -> void {
  for (const auto& comment : comments) {
    comments_.push_back(comment);
  }
}

auto MrfHeader::comments() const -> const std::vector<std::string>& {
  return comments_;
}

auto MrfHeader::HasColumn(MrfColumn column) const -> bool {
  return present_columns_.contains(column);
}

auto MrfHeader::AddColumn(MrfColumn column) -> void {
  if (present_columns_.contains(column)) {
    return;
  }
  columns_.push_back(column);
  present_columns_.insert(column);
}

auto MrfHeader::columns() const -> const std::vector<MrfColumn>& {
  return columns_;
}

auto MrfHeader::string() const -> std::string {
  std::vector<std::string> column_names;
  for (const auto& column : columns_) {
    switch (column) {
      case MrfColumn::kAlignmentBlocks:
        column_names.push_back(kColumnNameAlignmentBlocks);
        break;
      case MrfColumn::kSequence:
        column_names.push_back(kColumnNameSequence);
        break;
      case MrfColumn::kQualityScores:
        column_names.push_back(kColumnNameQualityScores);
        break;
      default:  // MrfColumn::kQueryId:
        column_names.push_back(kColumnNameQueryId);
        break;
    }
  }
  return absl::StrJoin(column_names, "\t");
}

auto MrfRead::Length() -> size_t {
  int length = 0;
  for (const auto& block : blocks) {
    length += (block.target_end - block.target_start + 1);
  }
  return length;
}

namespace {

auto ValidateRead(const MrfRead& read, const MrfHeader& header)
    -> absl::Status {
  if (read.blocks.size() <= 0) {
    return absl::InvalidArgumentError("Read must have one or more blocks");
  }
  for (int i = 0; i < read.blocks.size(); ++i) {
    const MrfBlock& block = read.blocks[i];
    if (block.target_name.empty()) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Block %d: missing target name", i));
    }
  }

  if (header.HasColumn(MrfColumn::kSequence) && read.sequence.empty()) {
    return absl::InvalidArgumentError(
        "Header contains Sequence column but sequence is empty");
  }
  if (header.HasColumn(MrfColumn::kQualityScores) &&
      read.quality_scores.empty()) {
    return absl::InvalidArgumentError(
        "Header contains QualityScores column but quality_scores is empty");
  }
  if (header.HasColumn(MrfColumn::kQueryId) && read.query_id.empty()) {
    return absl::InvalidArgumentError(
        "Header contains QueryId column but query_id is empty");
  }
  return absl::OkStatus();
}

}  // namespace

auto MrfEntry::Validate(const MrfHeader& header) -> absl::Status {
  RETURN_IF_ERROR(ValidateRead(read1, header));
  if (is_paired_end) {
    RETURN_IF_ERROR(ValidateRead(read2, header));
  }
  return absl::OkStatus();
}

}  // namespace bio
