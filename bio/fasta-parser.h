#ifndef BIO_FASTA_PARSER_H_
#define BIO_FASTA_PARSER_H_

#include <memory>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "prim/file/file.h"

namespace bio {

// Parser for FASTA files.
class FastaParser {
 public:
  explicit FastaParser(std::unique_ptr<prim::File> file)
      : file_(std::move(file)) {}

  // Constructs a new FastaParser from the specified path.
  static absl::StatusOr<std::unique_ptr<FastaParser>> New(
      absl::string_view path);

  // Returns the next sequence from the file.
  absl::StatusOr<Seq*> NextSequence();

  // Returns a vector of all sequences in the file.
  absl::StatusOr<std::vector<Seq>> ReadAllSequences();

 private:
  std::unique_ptr<prim::File> file_;
};

}  // namespace bio

#endif  // BIO_FASTA_PARSER_H_
