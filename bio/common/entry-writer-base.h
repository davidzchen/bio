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

#ifndef BIO_COMMON_ENTRY_WRITER_BASE_H_
#define BIO_COMMON_ENTRY_WRITER_BASE_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"

namespace bio {

// Template class for Writers that write Entries to a file.
template <typename Writer, typename Entry>
class EntryWriterBase {
 public:
  explicit EntryWriterBase(absl::Nonnull<abxl::File*> file) : file_(file) {}

  ~EntryWriterBase() = default;

  // Constructs a new writer from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<Writer>> {
    abxl::File* file;
    RETURN_IF_ERROR(abxl::file::Open(path, "w", &file, abxl::file::Defaults()));
    return std::make_unique<Writer>(file);
  }

  // Constructs a new writer from the specified file path or terminates the
  // program if constructing the writer fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<Writer> {
    absl::StatusOr<std::unique_ptr<Writer>> writer = New(path);
    CHECK_OK(writer.status());
    return std::move(writer.value());
  }

  // Writes the provided entries to the file.
  auto Write(const std::vector<Entry>& entries) -> absl::Status {
    for (const auto& entry : entries) {
      RETURN_IF_ERROR(Write(entry));
    }
    return absl::OkStatus();
  }

  // Writes the provided entry to the file.
  auto Write(const Entry& entry) -> absl::Status {
    const std::string str = absl::StrCat(entry.string(), "\n");
    size_t size = file_->WriteString(str);
    if (size != str.size()) {
      return absl::DataLossError(absl::StrFormat(
          "Expected to write %d bytes but wrote %d", str.size(), size));
    }
    return absl::OkStatus();
  }

  // Closes the file.
  auto Close() -> absl::Status { return file_->Close(abxl::file::Defaults()); }

 private:
  abxl::File* file_;
};

}  // namespace bio

#endif  // BIO_COMMON_ENTRY_WRITER_BASE_H_
