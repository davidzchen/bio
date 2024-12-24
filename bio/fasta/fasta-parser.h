#ifndef BIO_FASTA_FASTA_PARSER_H_
#define BIO_FASTA_FASTA_PARSER_H_

#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/file/filelineiter.h"
#include "bio/common/sequence.h"

namespace bio {

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
class FastaParser {
 public:
  // N.B. `file` is automatically closed by FileLines.
  explicit FastaParser(absl::Nonnull<abxl::File*> file)
      : file_(file),
        file_lines_(abxl::FileLines(file_->filename(), file_)),
        it_(file_lines_.begin()) {}

  ~FastaParser() = default;

  // Constructs a new FastaParser from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastaParser>>;

  // Returns the next sequence from the file.
  auto NextSequence(bool truncate_name = false)
      -> std::optional<std::unique_ptr<Sequence>>;

  // Returns a vector of all sequences in the file.
  auto ReadAllSequences(bool truncate_name = false)
      -> std::vector<std::unique_ptr<Sequence>>;

 private:
  // Returns the next line or std::nullopt if EOF has been reached. If there are
  // saved lines from calls to PutBack(), these lines will be returned first.
  auto NextLine() -> std::optional<std::string>;

  // Saves the line in the saved_lines_ queue.
  auto PutBack(std::string line) -> void;

  abxl::File* file_;
  abxl::FileLines file_lines_;
  abxl::FileLineIterator it_;
  std::queue<std::string> saved_lines_;
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_PARSER_H_
