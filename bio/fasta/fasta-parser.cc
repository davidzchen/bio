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

#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "absl/strings/match.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"
#include "bio/common/strings.h"
#include "bio/fasta/fasta.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

static constexpr char kDescriptionPrefix[] = ">";

}  // namespace

auto FastaParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastaParser>> {
  gxl::File* file;
  RETURN_IF_ERROR(gxl::Open(path, "r", &file, gxl::file::Defaults()));
  return std::make_unique<FastaParser>(file);
}

auto FastaParser::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastaParser> {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

auto FastaParser::Next(bool truncate_name)
    -> std::optional<std::unique_ptr<FastaSequence>> {
  if (eof()) {
    return std::nullopt;
  }

  int count = 0;
  auto sequence = std::make_unique<FastaSequence>();
  std::string buffer;
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }

    if (absl::StartsWith(*line, kDescriptionPrefix)) {
      ++count;
      if (count == 1) {
        // Save the description line.
        const std::string name = truncate_name ? FirstWord(*line) : *line;
        sequence->name = absl::StripPrefix(name, kDescriptionPrefix);
        continue;
      } else if (count == 2) {
        // The second occurrence we find a line starting with ">". This
        // denotes that a new Sequence is starting.
        sequence->sequence = buffer;
        PutBack(*line);
        return sequence;
      }
    }
    absl::StrAppend(&buffer, *line);
  }

  sequence->sequence = buffer;
  return sequence;
}

auto FastaParser::All(bool truncate_name)
    -> std::vector<std::unique_ptr<FastaSequence>> {
  std::vector<std::unique_ptr<FastaSequence>> sequences;
  while (true) {
    std::optional<std::unique_ptr<FastaSequence>> sequence =
        Next(truncate_name);
    if (!sequence.has_value()) {
      break;
    }
    sequences.push_back(std::move(sequence.value()));
  }
  return sequences;
}

}  // namespace bio
