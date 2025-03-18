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

#ifndef BIO_SAM_SAM_WRITER_H_
#define BIO_SAM_SAM_WRITER_H_

#include "absl/base/nullability.h"
#include "bio/common/entry-writer-base.h"
#include "bio/sam/sam.h"
#include "gxl/file/file.h"

namespace bio {

// Writer for SAM files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<SamWriter>> writer_or =
//     SamWriter::New("path/to/out.sam");
// if (!writer_or.ok()) {
//   // Handle error
// }
// std::unique_ptr<SamWriter> writer = std::move(writer_or.value());
//
// const std::vector<SamEntry> entries = {/* SAM entries */};
// absl::Status status = writer->Write(entries);
// if (!status.ok()) {
//   // Handle error
// }
//
// const SamEntry entry = {/* SAM entry */};
// status = writer->Write(entry);
// if (!status.ok()) {
//   // Handle error
// }
// ```
//
// Or, if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<SamWriter> writer,
//                  SamWriter::New("path/to/out.sam"));
// const std::vector<SamEntry> entries = {/* SAM entries */};
// RETURN_IF_ERROR(writer->Write(entries));
//
// const SamEntry entry = {/* SAM entry */};
// RETURN_IF_ERROR(writer->Write(entry));
// ```
class SamWriter : public EntryWriterBase<SamWriter, SamEntry> {
 public:
  explicit SamWriter(absl::Nonnull<gxl::File*> file) : EntryWriterBase(file) {}
};

}  // namespace bio

#endif  // BIO_SAM_SAM_WRITER_H_
