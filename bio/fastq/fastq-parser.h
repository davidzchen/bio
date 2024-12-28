#ifndef BIO_FASTQ_FASTQ_PARSER_H_
#define BIO_FASTQ_FASTQ_PARSER_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/common/line-parser-base.h"

namespace bio {

// Contains data from a FASTQ file for a single sequence.
struct FastqSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // The quality values for the sequence.
  std::string quality;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }
};

// Parser for FASTQ files.
//
// See https://maq.sourceforge.net/fastq.shtml
//
// auto parser = FastqParser::New("path/to/file.fastq");
// while (!parser.eof()) {
//   absl::StatusOr<std::unique_ptr<FastqSequence>> sequence_or =
//       parser->NextSequence(/*truncate_name=*/true);
//   if (!sequence_or.ok()) {
//     // Handle error.
//   }
//   std::unique_ptr<FastqSequence> sequence = std::move(sequence_or.value());
//   // Do stuff with sequence.
// }
//
// Or if using status macros:
//
// auto parser = FastqParser::New("path/to/file.fastq");
// while (!parser.eof()) {
//   ASSIGN_OR_RETURN(std::unique_ptr<FastqSequence> sequence,
//                    parser->NextSequence(/*truncate_name=*/true));
//   // Do stuff with sequence.
// }
class FastqParser : public LineParserBase {
 public:
  explicit FastqParser(absl::Nonnull<abxl::File*> file)
      : LineParserBase(file) {}

  ~FastqParser() = default;

  // Constructs a new FastqParser from the specified file path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastqParser>>;

  // Returns the next FASTQ entry from the file.
  //
  // Returns:
  //   * OK if the sequence was read successfully.
  //   * OUT_OF_RANGE if EOF has been reached
  auto NextSequence(bool truncate_name = false)
      -> absl::StatusOr<std::unique_ptr<FastqSequence>>;

  // Returns a vector of all sequences in the file.
  auto ReadAllSequences(bool truncate_name = false)
      -> absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>>;
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_PARSER_H_
