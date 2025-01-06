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

#include "bio/bedgraph/bedgraph-parser.h"

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
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/bedgraph/bedgraph.h"

namespace bio {
namespace {

namespace file = abxl::file;

// Number of fields expected for each line.
static constexpr int kNumFields = 4;

// Prefixes for non-feature lines to skip.
static constexpr char kCommentPrefix[] = "#";
static constexpr char kTrackPrefix[] = "track";
static constexpr char kBrowserPrefix[] = "browser";

}  // namespace

auto BedGraphParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<BedGraphParser>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<BedGraphParser>(file);
}

auto BedGraphParser::NewOrDie(absl::string_view path)
    -> std::unique_ptr<BedGraphParser> {
  absl::StatusOr<std::unique_ptr<BedGraphParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

namespace {

auto ParseUInt64(size_t line_number, absl::string_view str,
                 absl::string_view field) -> absl::StatusOr<uint64_t> {
  uint64_t value = 0;
  if (!absl::SimpleAtoi(str, &value)) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Invalid %s format: '%s'", line_number, field, str));
  }
  return value;
}

auto ParseDouble(size_t line_number, absl::string_view str,
                 absl::string_view field) -> absl::StatusOr<double> {
  double value = 0;
  if (!absl::SimpleAtod(str, &value)) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Invalid %s format: '%s'", line_number, field, str));
  }
  return value;
}

}  // namespace

auto BedGraphParser::NextEntry()
    -> absl::StatusOr<std::unique_ptr<BedGraphEntry>> {
  if (eof()) {
    return nullptr;
  }
  auto entry = std::make_unique<BedGraphEntry>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    ++line_number_;
    if (line->empty() || absl::StartsWith(*line, kCommentPrefix) ||
        absl::StartsWith(*line, kTrackPrefix) ||
        absl::StartsWith(*line, kBrowserPrefix)) {
      continue;
    }

    const std::vector<std::string> parts = absl::StrSplit(*line, '\t');
    if (parts.size() != kNumFields) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected 3 fields but got %d", kNumFields, parts.size()));
    }

    entry->chromosome = parts[0];
    ASSIGN_OR_RETURN(entry->start,
                     ParseUInt64(line_number_, parts[1], "start"));
    ASSIGN_OR_RETURN(entry->end, ParseUInt64(line_number_, parts[2], "end"));
    ASSIGN_OR_RETURN(entry->value,
                     ParseDouble(line_number_, parts[3], "value"));
    break;
  }
  return entry;
}

auto BedGraphParser::AllEntries()
    -> absl::StatusOr<std::vector<std::unique_ptr<BedGraphEntry>>> {
  std::vector<std::unique_ptr<BedGraphEntry>> entries;
  while (!eof()) {
    ASSIGN_OR_RETURN(std::unique_ptr<BedGraphEntry> entry, NextEntry());
    entries.push_back(std::move(entry));
  }
  return entries;
}

}  // namespace bio
