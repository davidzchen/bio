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
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "bio/bedgraph/bedgraph.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

// Number of fields expected for each line.
static constexpr int kNumFields = 4;

// Prefixes for non-feature lines to skip.
static constexpr char kCommentPrefix[] = "#";
static constexpr char kTrackPrefix[] = "track";
static constexpr char kBrowserPrefix[] = "browser";

}  // namespace

auto BedGraphParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<BedGraphParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(gxl::Open(path, "r", &file, gxl::file::Defaults()));
  return std::make_unique<BedGraphParser>(file);
}

auto BedGraphParser::NewOrDie(absl::string_view path)
    -> std::unique_ptr<BedGraphParser> {
  absl::StatusOr<std::unique_ptr<BedGraphParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

auto BedGraphParser::Next() -> absl::StatusOr<std::unique_ptr<BedGraphEntry>> {
  if (eof()) {
    return nullptr;
  }
  auto entry = std::make_unique<BedGraphEntry>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
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
    ASSIGN_OR_RETURN(entry->start, ParseInt<uint64_t>(parts[1], "start"));
    ASSIGN_OR_RETURN(entry->end, ParseInt<uint64_t>(parts[2], "end"));
    ASSIGN_OR_RETURN(entry->value, ParseDouble(parts[3], "value"));
    break;
  }
  return entry;
}

}  // namespace bio
