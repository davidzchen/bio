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

#ifndef BIO_BED_BED_WRITER_H_
#define BIO_BED_BED_WRITER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/bed/bed.h"

namespace bio {

// Writer for BED files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<BedWriter>> writer_or =
//     BedWriter::New("path/to/out.bed");
// if (!writer_or.ok()) {
//   // Handle error.
// }
// std::unique_ptr<BedWriter> writer = std::move(writer_or.value());
//
// std::vector<BedEntry> entries = {/*BED entries*/};
// absl::Status status = writer->Write(sequences);
// if (!status.ok()) {
//   // Handle error.
// }
//
// BedEntry entry = {/*BED entry*/};
// status = writer->Write(sequence);
// if (!status.ok()) {
//   // Handle error.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<BedWriter> writer,
//                  BedWriter::New("path/to/out.fastq"));
// std::vector<BedEntry> entries = {/*BED entries*/};
// RETURN_IF_ERROR(writer->Write(entries));
//
// BedEntry entry = {/*BED entry*/};
// RETURN_IF_ERROR(writer->Write(entry));
// ```
class BedWriter {
 public:
  explicit BedWriter(absl::Nonnull<abxl::File*> file) : file_(file) {}

  ~BedWriter() = default;

  // Constructs a new FastaWriter from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<BedWriter>>;

  // Constructs a new FastaWriter from the specified file path or terminates the
  // program if constructing the writer fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<BedWriter>;

  // Writes the provided BED entries to the file.
  auto Write(const std::vector<BedEntry>& entries) -> absl::Status;

  // Writes the provided BED entry to the file.
  auto Write(const BedEntry& entry) -> absl::Status;

  // Closes the file.
  auto Close() -> absl::Status;

 private:
  abxl::File* file_;
};

}  // namespace bio

#endif  // BIO_BED_BED_WRITER_H_
