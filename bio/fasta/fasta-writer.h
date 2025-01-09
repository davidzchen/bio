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

#ifndef BIO_FASTA_FASTA_WRITER_H_
#define BIO_FASTA_FASTA_WRITER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "bio/common/entry-writer-base.h"
#include "bio/fasta/fasta.h"
#include "gxl/file/file.h"

namespace bio {

// Writer for FASTA files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<FastaWriter>> writer_or =
//     FastaWriter::New("path/to/out.fasta");
// if (!writer_or.ok()) {
//   // Handle error.
// }
// std::unique_ptr<FastaWriter> writer = std::move(writer_or.value());
//
// std::vector<FastaSequence> sequences = {/*FASTA sequences*/};
// absl::Status status = writer->Write(sequences);
// if (!status.ok()) {
//   // Handle error.
// }
//
// FastaSequence sequence = {/*FASTA sequence*/};
// status = writer->Write(sequence);
// if (!status.ok()) {
//   // Handle error.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<FastaWriter> writer,
//                  FastaWriter::New("path/to/out.fasta"));
//
// std::vector<FastaSequence> sequences = {/*FASTA sequences*/};
// RETURN_IF_ERROR(writer->Write(sequences));
//
// FastaSequence sequence = {/*FASTA sequence*/};
// RETURN_IF_ERROR(writer->Write(sequence));
// ```
class FastaWriter : public EntryWriterBase<FastaWriter, FastaSequence> {
 public:
  explicit FastaWriter(absl::Nonnull<gxl::File*> file)
      : EntryWriterBase(file) {}
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_WRITER_H_
