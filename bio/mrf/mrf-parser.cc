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

#include "bio/mrf/mrf-parser.h"

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"
#include "bio/mrf/mrf.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

namespace file = gxl::file;

static constexpr char kCommentMarker[] = "#";

// Number of fields in an alignment block.
static constexpr int kNumBlockFields = 6;

}  // namespace

auto MrfParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<MrfParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<MrfParser>(file);
}

auto MrfParser::NewOrDie(absl::string_view path) -> std::unique_ptr<MrfParser> {
  absl::StatusOr<std::unique_ptr<MrfParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

namespace {

auto ParseComment(absl::string_view raw) -> std::string {
  absl::string_view stripped = absl::StripPrefix(raw, kCommentMarker);
  return std::string(absl::StripAsciiWhitespace(stripped));
}

}  // namespace

auto MrfParser::Start() -> absl::StatusOr<std::unique_ptr<MrfHeader>> {
  started_ = true;
  if (eof()) {
    return nullptr;
  }
  std::vector<std::string> comments;
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (absl::StartsWith(*line, kCommentMarker)) {
      comments.push_back(ParseComment(*line));
      continue;
    }
    ASSIGN_OR_RETURN(std::unique_ptr<MrfHeader> header,
                     MrfHeader::Parse(*line));
    header->AddComments(comments);
    columns_ = header->columns();
    return header;
  }
  return absl::InternalError("Should not be reached");
}

namespace {

auto IsPairedEnd(absl::string_view line) -> bool {
  return absl::StrContainsIgnoreCase(line, "|");
}

}  // namespace

auto MrfParser::ParseSequence(MrfEntry* entry, absl::string_view column)
    -> absl::Status {
  if (entry->is_paired_end) {
    std::vector<std::string> tokens = absl::StrSplit(column, "|");
    if (tokens.size() != 2) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: invalid number of Sequence tokens: %s",
                          line_number(), column));
    }
    entry->read1.sequence = tokens[0];
    entry->read2.sequence = tokens[1];
  } else {
    entry->read1.sequence = std::string(column);
  }
  return absl::OkStatus();
}

auto MrfParser::ParseQualityScores(MrfEntry* entry, absl::string_view column)
    -> absl::Status {
  if (entry->is_paired_end) {
    std::vector<std::string> tokens = absl::StrSplit(column, "|");
    if (tokens.size() != 2) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: invalid number of QualityScores tokens: %s",
                          line_number(), column));
    }
    entry->read1.quality_scores = tokens[0];
    entry->read2.quality_scores = tokens[1];
  } else {
    entry->read1.quality_scores = std::string(column);
  }
  return absl::OkStatus();
}

auto MrfParser::ParseQueryId(MrfEntry* entry, absl::string_view column)
    -> absl::Status {
  if (entry->is_paired_end) {
    std::vector<std::string> tokens = absl::StrSplit(column, "|");
    if (tokens.size() != 2) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: invalid number of QueryId tokens: %s",
                          line_number(), column));
    }
    entry->read1.query_id = tokens[0];
    entry->read2.query_id = tokens[1];
  } else {
    entry->read1.query_id = std::string(column);
  }
  return absl::OkStatus();
}

auto MrfParser::ParseAlignmentBlocks(MrfEntry* entry, absl::string_view column)
    -> absl::Status {
  if (entry->is_paired_end) {
    std::vector<std::string> tokens = absl::StrSplit(column, "|");
    if (tokens.size() != 2) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: invalid number of AlignmentBlock tokens: %s", line_number(),
          column));
    }
    RETURN_IF_ERROR(ProcessBlocks(&entry->read1, tokens[0]));
    RETURN_IF_ERROR(ProcessBlocks(&entry->read2, tokens[1]));
  } else {
    RETURN_IF_ERROR(ProcessBlocks(&entry->read1, column));
  }
  return absl::OkStatus();
}

auto MrfParser::ProcessBlocks(MrfRead* read, absl::string_view token)
    -> absl::Status {
  std::vector<std::string> blocks = absl::StrSplit(token, ",");
  for (const auto& block : blocks) {
    std::vector<std::string> fields = absl::StrSplit(block, ":");
    if (fields.size() != kNumBlockFields) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: Invalid number of fields for block: '%s'",
                          line_number(), block));
    }
    MrfBlock mrf_block;
    mrf_block.target_name = fields[0];
    ASSIGN_OR_RETURN(mrf_block.strand, ParseStrand(fields[1]));
    ASSIGN_OR_RETURN(mrf_block.target_start,
                     ParseInt<uint64_t>(fields[2], "target start"));
    ASSIGN_OR_RETURN(mrf_block.target_end,
                     ParseInt<uint64_t>(fields[3], "target end"));
    ASSIGN_OR_RETURN(mrf_block.query_start,
                     ParseInt<uint64_t>(fields[4], "query start"));
    ASSIGN_OR_RETURN(mrf_block.query_end,
                     ParseInt<uint64_t>(fields[5], "query end"));
    read->blocks.push_back(mrf_block);
  }
  return absl::OkStatus();
}

auto MrfParser::Next() -> absl::StatusOr<std::unique_ptr<MrfEntry>> {
  if (!started_) {
    return absl::FailedPreconditionError(
        "Start() must be called before Next()");
  }
  if (eof()) {
    return nullptr;
  }

  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (absl::StartsWith(*line, kCommentMarker)) {
      // There should not be any comments after the header line, but we skip
      // them in case there are.
      continue;
    }

    std::vector<std::string> columns = absl::StrSplit(*line, "\t");
    if (columns.size() != columns_.size()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: number of columns do not match header", line_number()));
    }

    auto entry = std::make_unique<MrfEntry>();
    entry->is_paired_end = IsPairedEnd(*line);
    for (int i = 0; i < columns.size(); ++i) {
      switch (columns_[i]) {
        case MrfColumn::kAlignmentBlocks:
          RETURN_IF_ERROR(ParseAlignmentBlocks(entry.get(), columns[i]));
          break;
        case MrfColumn::kSequence:
          RETURN_IF_ERROR(ParseSequence(entry.get(), columns[i]));
          break;
        case MrfColumn::kQualityScores:
          RETURN_IF_ERROR(ParseQualityScores(entry.get(), columns[i]));
          break;
        case MrfColumn::kQueryId:
          RETURN_IF_ERROR(ParseQueryId(entry.get(), columns[i]));
          break;
        default:
          return absl::InternalError("Invalid column type found");
      }
    }
    return entry;
  }
  return absl::InternalError("Should not be reached");
}

}  // namespace bio
