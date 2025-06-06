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

#include "bio/fastq/fastq-parser.h"

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
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"
#include "bio/common/strings.h"
#include "bio/fastq/fastq.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

static constexpr char kIdentifierPrefix[] = "@";
static constexpr char kQualityIdPrefix[] = "+";

}  // namespace

auto FastqParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastqParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(gxl::Open(path, "r", &file, gxl::file::Defaults()));
  return std::make_unique<FastqParser>(file);
}

auto FastqParser::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastqParser> {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

auto FastqParser::Next(bool truncate_name)
    -> absl::StatusOr<std::unique_ptr<FastqSequence>> {
  if (eof()) {
    return nullptr;
  }

  auto sequence = std::make_unique<FastqSequence>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (!absl::StartsWith(*line, kIdentifierPrefix)) {
      continue;
    }

    // Read sequence identifier line.
    const std::string name = truncate_name ? FirstWord(*line) : *line;
    sequence->name = absl::StripPrefix(name, kIdentifierPrefix);

    // Read sequence line.
    std::optional<std::string> sequence_line = NextLine();
    if (!sequence_line.has_value()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected sequence line but got EOF", line_number()));
    }
    sequence->sequence = std::move(sequence_line.value());

    // Read sequence name line.
    std::optional<std::string> quality_id_line = NextLine();
    if (!quality_id_line.has_value()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected quality ID line but got EOF", line_number()));
    }
    if (!absl::StartsWith(*quality_id_line, kQualityIdPrefix)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Line %d: Expected quality ID: '%s' or '+%s'",
                          line_number(), kQualityIdPrefix, sequence->name));
    }

    // Read quality line.
    std::optional<std::string> quality_line = NextLine();
    if (!quality_line.has_value()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Expected quality line but got EOF", line_number()));
    }
    sequence->quality = std::move(quality_line.value());
    if (sequence->quality.size() != sequence->sequence.size()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Sequence line length %d does not match quality line length "
          "%d",
          line_number(), sequence->sequence.size(), sequence->quality.size()));
    }

    break;
  }
  return sequence;
}

}  // namespace bio
