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

#ifndef BIO_FASTQ_FASTQ_PARSER_H_
#define BIO_FASTQ_FASTQ_PARSER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/common/line-parser-base.h"
#include "bio/fastq/fastq.h"
#include "gxl/file/file.h"

namespace bio {

// Parser for FASTQ files.
//
// See https://maq.sourceforge.net/fastq.shtml
//
// Example usage:
//
// ```cpp
// absl::StatusOr<std::unique_ptr<FastqParser>> parser_or =
//     FastqParser::New("path/to/file.fastq");
// if (!parser_or.ok()) {
//   // handle error.
// }
// std::unique_ptr<FastqParser> parser = std::move(parser_or.value());
// while (!parser->eof()) {
//   absl::StatusOr<std::unique_ptr<FastqSequence>> sequence_or =
//       parser->NextSequence(/*truncate_name=*/true);
//   if (!sequence_or.ok()) {
//     // Handle error.
//   }
//   std::unique_ptr<FastqSequence> sequence = std::move(sequence_or.value());
//   // Do stuff with sequence.
// }
// ```
//
// Or if using status macros:
//
// ```cpp
// ASSIGN_OR_RETURN(std::unique_ptr<FastqParser> parser,
//                  FastqParser::New("path/to/file.fastq"));
// while (!parser->eof()) {
//   ASSIGN_OR_RETURN(std::unique_ptr<FastqSequence> sequence,
//                    parser->NextSequence(/*truncate_name=*/true));
//   // Do stuff with sequence.
// }
// ```
class FastqParser : public LineParserBase {
 public:
  explicit FastqParser(absl::Nonnull<gxl::File*> file) : LineParserBase(file) {}

  ~FastqParser() = default;

  // Constructs a new FastqParser from the specified file path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<FastqParser>>;

  // Constructs a new FastqParser from the specified file path or terminates the
  // program if constructing the parser fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<FastqParser>;

  // Returns the next FASTQ entry from the file.
  auto NextSequence(bool truncate_name = false)
      -> absl::StatusOr<std::unique_ptr<FastqSequence>>;

  // Returns a vector of all sequences in the file.
  auto ReadAllSequences(bool truncate_name = false)
      -> absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>>;
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_PARSER_H_
