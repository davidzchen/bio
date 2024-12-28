#ifndef BIO_FASTA_FASTA_PARSER_H_
#define BIO_FASTA_FASTA_PARSER_H_

#include <cstdlib>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/common/line-parser-base.h"

namespace bio {

// Contains data from a FASTA file for a single sequence.
struct FastaSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }
};

// Parser for FASTA files.
//
// See https://en.wikipedia.org/wiki/FASTA_format for more details.
//
// Example usage:
//
// auto parser = FastaParser::New("path/to/file.fasta");
// while (true) {
//   std::optional<std::unique_ptr<Sequence>> sequence =
//       parser->NextSequence(/*truncate_name=*/true);
//   if (!sequence.has_value()) {
//     break;
//   }
//
//   // Do stuff with `sequence`
// }
class FastaParser : public LineParserBase {
 public:
  // N.B. `file` is automatically closed by FileLines.
  explicit FastaParser(absl::Nonnull<abxl::File*> file)
      : LineParserBase(file) {}

  ~FastaParser() = default;

  // Constructs a new FastaParser from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastaParser>>;

  // Returns the next sequence from the file.
  auto NextSequence(bool truncate_name = false)
      -> std::optional<std::unique_ptr<FastaSequence>>;

  // Returns a vector of all sequences in the file.
  auto ReadAllSequences(bool truncate_name = false)
      -> std::vector<std::unique_ptr<FastaSequence>>;
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_PARSER_H_
