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

#include "bio/sam/sam-parser.h"

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
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "bio/sam/cigar-parser.h"
#include "bio/sam/sam.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

static constexpr char kHeaderLinePrefix[] = "@";
static constexpr size_t kMinSamFields = 11;

}  // namespace

auto SamParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<SamParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(gxl::Open(path, "r", &file, gxl::file::Defaults()));
  return std::make_unique<SamParser>(file);
}

auto SamParser::NewOrDie(absl::string_view path) -> std::unique_ptr<SamParser> {
  absl::StatusOr<std::unique_ptr<SamParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

auto SamParser::Next() -> absl::StatusOr<std::unique_ptr<SamEntry>> {
  if (eof()) {
    return nullptr;
  }

  auto entry = std::make_unique<SamEntry>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (absl::StartsWith(*line, kHeaderLinePrefix)) {
      continue;
    }

    const std::vector<std::string> fields = absl::StrSplit(*line, "\t");
    if (fields.size() < kMinSamFields) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Invalid number of fields: '%s'", line_number(), *line));
    }
    entry->qname = fields[0];
    ASSIGN_OR_RETURN(entry->flags, ParseInt<uint64_t>(fields[1], "flags"));
    entry->rname = fields[2];
    ASSIGN_OR_RETURN(entry->pos, ParseInt<uint32_t>(fields[3], "pos"));
    ASSIGN_OR_RETURN(entry->mapq, ParseUInt8(fields[4], "mapq"));

    CigarParser cigar_parser;
    ASSIGN_OR_RETURN(entry->cigar, cigar_parser.Parse(fields[5]));
    entry->rnext = fields[6];
    ASSIGN_OR_RETURN(entry->pnext, ParseInt<uint32_t>(fields[7], "pnext"));
    ASSIGN_OR_RETURN(entry->tlen, ParseInt<int32_t>(fields[8], "tlen"));

    if (fields[9] != "*") {
      entry->seq = fields[9];
    }
    if (fields[10] != "*") {
      entry->qual = fields[10];
    }

    if (fields.size() > kMinSamFields) {
      for (int i = kMinSamFields; i < fields.size(); ++i) {
        entry->tags.push_back(fields[i]);
      }
    }
    return entry;
  }
  return nullptr;
}

}  // namespace bio
