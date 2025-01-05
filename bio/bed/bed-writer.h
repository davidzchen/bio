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
#include "abxl/file/file.h"
#include "bio/bed/bed.h"
#include "bio/common/entry-writer-base.h"

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
class BedWriter : public EntryWriterBase<BedWriter, BedEntry> {
 public:
  explicit BedWriter(absl::Nonnull<abxl::File*> file) : EntryWriterBase(file) {}
};

}  // namespace bio

#endif  // BIO_BED_BED_WRITER_H_
