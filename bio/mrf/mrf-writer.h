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

#ifndef BIO_MRF_MRF_WRITER_H_
#define BIO_MRF_MRF_WRITER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/mrf/mrf.h"
#include "gxl/file/file.h"

namespace bio {

class MrfWriter {
 public:
  explicit MrfWriter(absl::Nonnull<gxl::File*> file,
                     absl::Nonnull<MrfHeader*> header)
      : file_(file), header_(header), started_(false) {}

  ~MrfWriter() = default;

  // Constructs a new writer from the specified path.
  static auto New(absl::string_view path, absl::Nonnull<MrfHeader*> header)
      -> absl::StatusOr<std::unique_ptr<MrfWriter>>;

  // Constructs a new writer from the specified file path or terminates the
  // program if constructing the writer fails.
  static auto NewOrDie(absl::string_view path, absl::Nonnull<MrfHeader*> header)
      -> std::unique_ptr<MrfWriter>;

  // Starts the writer by writing the header. `Start` must be called before
  // any of the `Write` methods.
  auto Start() -> absl::Status;

  // Writes the provided MRF entries to the file.
  auto Write(const std::vector<MrfEntry>& entries) -> absl::Status;

  // Writes the provided pointer to the entry to the file.
  auto Write(absl::Nonnull<const MrfEntry*> entry) -> absl::Status;

  // Writes the provided entry to the file.
  auto Write(const MrfEntry& entry) -> absl::Status;

  // Closes the file.
  auto Close() -> absl::Status;

 private:
  gxl::File* file_;
  MrfHeader* header_;  // Not owned
  bool started_;
};

}  // namespace bio

#endif  // BIO_MRF_MRF_WRITER_H_
