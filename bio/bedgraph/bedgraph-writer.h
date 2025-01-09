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

#ifndef BIO_BEDGRAPH_BEDGRAPH_WriTER_H_
#define BIO_BEDGRAPH_BEDGRAPH_WriTER_H_

#include "absl/base/nullability.h"
#include "bio/bedgraph/bedgraph.h"
#include "bio/common/entry-writer-base.h"
#include "gxl/file/file.h"

namespace bio {

// Writer for BedGraph files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<BedGraphWriter>> writer_or =
//     BedWriter::New("path/to/out.bedgraph");
// if (!writer_or.ok()) {
//   // Handle error.
// }
// std::unique_ptr<BedGraphWriter> writer = std::move(writer_or.value());
//
// std::vector<BedGraphEntry> entries = {/*BED entries*/};
// absl::Status status = writer->Write(entry);
// if (!status.ok()) {
//   // Handle error.
// }
//
// BedGraphEntry entry = {/*BED entry*/};
// status = writer->Write(entry);
// if (!status.ok()) {
//   // Handle error.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<BedGraphWriter> writer,
//                  BedWriter::New("path/to/out.fastq"));
// std::vector<BedGraphEntry> entries = {/*BED entries*/};
// RETURN_IF_ERROR(writer->Write(entries));
//
// BedGraphEntry entry = {/*BED entry*/};
// RETURN_IF_ERROR(writer->Write(entry));
// ```
class BedGraphWriter : public EntryWriterBase<BedGraphWriter, BedGraphEntry> {
 public:
  explicit BedGraphWriter(absl::Nonnull<gxl::File*> file)
      : EntryWriterBase(file) {}
};

}  // namespace bio

#endif  // BIO_BEDGRAPH_BEDGRAPH_WriTER_H_
