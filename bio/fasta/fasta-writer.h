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
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/fasta/fasta.h"

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
class FastaWriter {
 public:
  explicit FastaWriter(absl::Nonnull<abxl::File*> file) : file_(file) {}

  ~FastaWriter() = default;

  // Constructs a new FastaWriter from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastaWriter>>;

  // Constructs a new FastaWriter from the specified file path or terminates the
  // program if constructing the writer fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<FastaWriter>;

  // Writes the provided sequences to the file.
  auto Write(const std::vector<FastaSequence>& sequences) -> absl::Status;

  // Writes the provided sequence to the file.
  auto Write(const FastaSequence& sequence) -> absl::Status;

  // Closes the file.
  auto Close() -> absl::Status;

 private:
  abxl::File* file_;
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_WRITER_H_
