#ifndef BIO_FASTQ_FASTQ_WRITER_H_
#define BIO_FASTQ_FASTQ_WRITER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/fastq/fastq.h"

namespace bio {

// Writer for FASTQ files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<FastqWriter>> writer_or =
//     FastqWriter::New("path/to/out.fastq");
// if (!writer_or.ok()) {
//   // Handle error
// }
// std::unique_ptr<FastqWriter> writer = std::move(writer_or.value());
//
// std::vector<FastqSequence> sequences = {/*FASTQ sequences*/};
// absl::Status status = writer->Write(sequences);
// if (!status.ok()) {
//   // Handle error.
// }
//
// FastqSequence ssequence = {/*FASTQ sequence*/};
// status = writer->Write(sequence);
// if (!status.ok()) {
//   // Handle error.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<FastqWriter> writer,
//                  FastqWriter::New("path/to/out.fastq"));
// std::vector<FastqSequence> sequences = {/*FASTQ sequences*/};
// RETURN_IF_ERROR(writer->Write(sequences));
//
// FastqSequence ssequence = {/*FASTQ sequence*/};
// RETURN_IF_ERROR(writer->Write(sequence));
// ```
class FastqWriter {
 public:
  explicit FastqWriter(absl::Nonnull<abxl::File*> file) : file_(file) {}

  ~FastqWriter() = default;

  // Constructs a new FastaWriter from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastqWriter>>;

  // Constructs a new FastaWriter from the specified file path or terminates the
  // program if constructing the writer fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<FastqWriter>;

  // Writes the provided sequences to the file.
  auto Write(const std::vector<FastqSequence>& sequences) -> absl::Status;

  // Writes the provided sequence to the file.
  auto Write(const FastqSequence& sequence) -> absl::Status;

  // Closes the file.
  auto Close() -> absl::Status;

 private:
  abxl::File* file_;
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_WRITER_H_
