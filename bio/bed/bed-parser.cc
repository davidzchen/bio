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

#include "bio/bed/bed-parser.h"

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
#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "bio/bed/bed.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

namespace file = gxl::file;

// The minimum number of BED fields.
static constexpr int kMinBedFields = 3;

// Prefixes for non-feature lines to skip.
static constexpr char kCommentPrefix[] = "#";
static constexpr char kTrackPrefix[] = "track";
static constexpr char kBrowserPrefix[] = "browser";

}  // namespace

auto BedParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<BedParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<BedParser>(file);
}

auto BedParser::NewOrDie(absl::string_view path) -> std::unique_ptr<BedParser> {
  absl::StatusOr<std::unique_ptr<BedParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

namespace {

auto ParseSubBlocks(size_t line_number, uint64_t block_count,
                    absl::string_view block_sizes,
                    absl::string_view block_starts)
    -> absl::StatusOr<std::vector<BedSubBlock>> {
  const std::vector<std::string> block_sizes_parts =
      absl::StrSplit(block_sizes, ',');
  const std::vector<std::string> block_starts_parts =
      absl::StrSplit(block_starts, ',');

  if (block_sizes_parts.size() != block_count) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: number of block sizes %d does not match block count %d",
        line_number, block_sizes_parts.size(), block_count));
  }
  if (block_starts_parts.size() != block_count) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: number of block starts %d does not match block count %d",
        line_number, block_starts_parts.size(), block_count));
  }

  std::vector<BedSubBlock> blocks;
  for (int i = 0; i < block_count; ++i) {
    uint64_t size = 0;
    if (!absl::SimpleAtoi(block_sizes_parts[i], &size)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: invalid block size format: '%s'",
                          line_number, block_sizes_parts[i]));
    }
    uint64_t start = 0;
    if (!absl::SimpleAtoi(block_starts_parts[i], &start)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: invalid block start format: '%s'",
                          line_number, block_starts_parts[i]));
    }
    blocks.push_back({.size = size, .start = start});
  }
  return blocks;
}

}  // namespace

auto BedParser::NextEntry() -> absl::StatusOr<std::unique_ptr<BedEntry>> {
  if (eof()) {
    return nullptr;
  }

  auto entry = std::make_unique<BedEntry>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty() || absl::StartsWith(*line, kCommentPrefix) ||
        absl::StartsWith(*line, kTrackPrefix) ||
        absl::StartsWith(*line, kBrowserPrefix)) {
      continue;
    }

    // Split line and check.
    const std::vector<std::string> parts = absl::StrSplit(*line, '\t');
    if (parts.size() < kMinBedFields) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected at least %d fields but got only %d", line_number(),
          kMinBedFields, parts.size()));
    }
    if (num_fields_ == 0) {
      // If this is the first line that contains a feature, save the number of
      // fields. All subsequent lines must contain the same number of fields.
      num_fields_ = parts.size();
    } else {
      if (parts.size() != num_fields_) {
        return absl::InvalidArgumentError(
            absl::StrFormat("line %d: Expected %d fields but got %d",
                            line_number(), num_fields_, parts.size()));
      }
    }

    // Parse required fields.
    entry->chromosome = parts[0];
    ASSIGN_OR_RETURN(entry->start,
                     ParseInt<uint64_t>(parts[1], "feature start"));
    ASSIGN_OR_RETURN(entry->end, ParseInt<uint64_t>(parts[2], "feature end"));

    // Parse optional fields.
    if (parts.size() < 4) {
      break;
    }
    entry->name = parts[3];

    if (parts.size() < 5) {
      break;
    }
    ASSIGN_OR_RETURN(entry->score, ParseInt<uint32_t>(parts[4], "score"));

    if (parts.size() < 6) {
      break;
    }
    ASSIGN_OR_RETURN(entry->strand, ParseStrand(parts[5]));

    if (parts.size() < 7) {
      break;
    }
    ASSIGN_OR_RETURN(entry->thick_start,
                     ParseInt<uint64_t>(parts[6], "thick start"));

    if (parts.size() < 8) {
      break;
    }
    ASSIGN_OR_RETURN(entry->thick_end,
                     ParseInt<uint64_t>(parts[7], "thick end"));

    if (parts.size() < 9) {
      break;
    }
    // TODO: Add validation for rgb
    entry->item_rgb = parts[8];

    // Block count, block sizes, and block starts are all required for BED12+
    // files.
    if (parts.size() < 10) {
      break;
    }
    if (parts.size() != 12) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected 12 fields but got %d. Block count, block sizes, "
          "and block starts are all required for BED12+",
          line_number(), parts.size()));
    }
    ASSIGN_OR_RETURN(const uint64_t block_count,
                     ParseInt<uint64_t>(parts[9], "block size"));
    const std::string block_sizes = parts[10];
    const std::string block_starts = parts[11];
    ASSIGN_OR_RETURN(
        entry->sub_blocks,
        ParseSubBlocks(line_number(), block_count, block_sizes, block_starts));
    break;
  }

  return entry;
}

auto BedParser::AllEntries()
    -> absl::StatusOr<std::vector<std::unique_ptr<BedEntry>>> {
  std::vector<std::unique_ptr<BedEntry>> entries;
  while (!eof()) {
    ASSIGN_OR_RETURN(std::unique_ptr<BedEntry> entry, NextEntry());
    entries.push_back(std::move(entry));
  }
  return entries;
}

}  // namespace bio
