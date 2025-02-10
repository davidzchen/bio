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
#include <exception>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/match.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "bio/sam/sam.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

namespace file = gxl::file;

static constexpr char kHeaderLinePrefix[] = "@";

static constexpr size_t kMinSamFields = 11;

}  // namespace

auto SamParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<SamParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<SamParser>(file);
}

auto SamParser::NewOrDie(absl::string_view path) -> std::unique_ptr<SamParser> {
  absl::StatusOr<std::unique_ptr<SamParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

namespace {

// TODO(dzc): Factor this and the same function from bed-parser out into a
// common library.
auto ParseUInt64(size_t line_number, absl::string_view str,
                 absl::string_view field) -> absl::StatusOr<uint64_t> {
  uint64_t value = 0;
  if (!absl::SimpleAtoi(str, &value)) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Invalid %s format: '%s'", line_number, field, str));
  }
  return value;
}

// TODO(dzc): Factor this and the same function from bed-parser out into a
// common library.
auto ParseUInt32(size_t line_number, absl::string_view str,
                 absl::string_view field) -> absl::StatusOr<uint32_t> {
  uint32_t value = 0;
  if (!absl::SimpleAtoi(str, &value)) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Invalid %s format: '%s'", line_number, field, str));
  }
  return value;
}

// TODO(dzc): Factor this and the same function from bed-parser out into a
// common library.
auto ParseUInt8(size_t line_number, absl::string_view str,
                absl::string_view field) -> absl::StatusOr<uint8_t> {
  try {
    size_t index = 0;
    int value = std::stoi(std::string(str), &index, 10);
    if (index != str.size()) {
      return absl::OutOfRangeError(
          absl::StrFormat("Line %d: value is out of range for a uint8_t: %s",
                          line_number, field));
    }

    if (value < 0 || value > std::numeric_limits<uint8_t>::max()) {
      return absl::OutOfRangeError(absl::StrFormat(
          "Line %d: value is out of range for a uint8_t", line_number));
    }

    return static_cast<uint8_t>(value);
  } catch (const std::exception& e) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Failed to parse '%s': %s", line_number, field, e.what()));
  }
  return absl::InternalError("Should not be reached");
}

// TODO(dzc): Factor this and the same function from bed-parser out into a
// common library.
auto ParseInt32(size_t line_number, absl::string_view str,
                absl::string_view field) -> absl::StatusOr<int32_t> {
  int32_t value = 0;
  if (!absl::SimpleAtoi(str, &value)) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Line %d: Invalid %s format: '%s'", line_number, field, str));
  }
  return value;
}

}  // namespace

auto SamParser::Next() -> absl::StatusOr<std::unique_ptr<SamEntry>> {
  if (eof()) {
    return nullptr;
  }

  auto entry = std::make_unique<SamEntry>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    ++line_number_;
    if (line->empty()) {
      continue;
    }
    if (absl::StartsWith(*line, kHeaderLinePrefix)) {
      continue;
    }

    const std::vector<std::string> fields = absl::StrSplit(*line, "\t");
    if (fields.size() < kMinSamFields) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Invalid number of fields: '%s'", line_number_, *line));
    }
    entry->qname = fields[0];
    ASSIGN_OR_RETURN(entry->flags,
                     ParseUInt64(line_number_, fields[1], "flags"));
    entry->rname = fields[2];
    ASSIGN_OR_RETURN(entry->pos, ParseUInt32(line_number_, fields[3], "pos"));
    ASSIGN_OR_RETURN(entry->mapq, ParseUInt8(line_number_, fields[4], "mapq"));
    entry->cigar = fields[5];
    entry->rnext = fields[6];
    ASSIGN_OR_RETURN(entry->pnext,
                     ParseUInt32(line_number_, fields[7], "pnext"));
    ASSIGN_OR_RETURN(entry->tlen, ParseInt32(line_number_, fields[8], "tlen"));

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

auto SamParser::All()
    -> absl::StatusOr<std::vector<std::unique_ptr<SamEntry>>> {
  std::vector<std::unique_ptr<SamEntry>> entries;
  while (!eof()) {
    ASSIGN_OR_RETURN(std::unique_ptr<SamEntry> entry, Next());
    entries.push_back(std::move(entry));
  }
  return entries;
}

}  // namespace bio
