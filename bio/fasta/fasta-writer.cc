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

#include "bio/fasta/fasta-writer.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/fasta/fasta.h"

namespace bio {
namespace {

namespace file = abxl::file;

}  // namespace

auto FastaWriter::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastaWriter>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "w", &file, file::Defaults()));
  return std::make_unique<FastaWriter>(file);
}

auto FastaWriter::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastaWriter> {
  absl::StatusOr<std::unique_ptr<FastaWriter>> writer = New(path);
  CHECK_OK(writer.status());
  return std::move(writer.value());
}

auto FastaWriter::Write(const std::vector<FastaSequence>& sequences)
    -> absl::Status {
  for (const auto& sequence : sequences) {
    RETURN_IF_ERROR(Write(sequence));
  }
  return absl::OkStatus();
}

auto FastaWriter::Write(const FastaSequence& sequence) -> absl::Status {
  const std::string seq = sequence.string();
  size_t size = file_->WriteString(seq);
  if (size != seq.size()) {
    return absl::DataLossError(absl::StrFormat(
        "Expected to write %d bytes but only wrote %d", seq.size(), size));
  }
  return absl::OkStatus();
}

auto FastaWriter::Close() -> absl::Status {
  return file_->Close(file::Defaults());
}

}  // namespace bio
