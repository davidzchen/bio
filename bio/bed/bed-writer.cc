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

#include "bio/bed/bed-writer.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/bed/bed.h"

namespace bio {
namespace {

namespace file = abxl::file;

}

auto BedWriter::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<BedWriter>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "w", &file, file::Defaults()));
  return std::make_unique<BedWriter>(file);
}

auto BedWriter::NewOrDie(absl::string_view path) -> std::unique_ptr<BedWriter> {
  absl::StatusOr<std::unique_ptr<BedWriter>> writer = New(path);
  CHECK_OK(writer.status());
  return std::move(writer.value());
}

auto BedWriter::Write(const std::vector<BedEntry>& entries) -> absl::Status {
  for (const auto& entry : entries) {
    RETURN_IF_ERROR(Write(entry));
  }
  return absl::OkStatus();
}

auto BedWriter::Write(const BedEntry& entry) -> absl::Status {
  const std::string seq = absl::StrCat(entry.string(), "\n");
  size_t size = file_->WriteString(seq);
  if (size != seq.size()) {
    return absl::DataLossError(absl::StrFormat(
        "Expected to write %d bytes but wrote %d", seq.size(), size));
  }
  return absl::OkStatus();
}

auto BedWriter::Close() -> absl::Status {
  return file_->Close(file::Defaults());
}

}  // namespace bio
