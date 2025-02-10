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

#ifndef BIO_SAM_SAM_PARSER_H_
#define BIO_SAM_SAM_PARSER_H_

#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/common/line-parser-base.h"
#include "bio/sam/sam.h"
#include "gxl/file/file.h"

namespace bio {

// Parser for SAM files.
//
// See https://samtools.github.io/hts-specs/SAMv1.pdf
//
// Example usage:
//
// ```cpp
// absl::StatusOr<std::unique_ptr<SamParser>> parser_or =
//     SamParser::New("path/to/file.sam");
// if (!parser_or.ok()) {
//   // Handle error.
// }
// std::unique_ptr<SamParser> parser = std::move(parser_or.value());
// while (!parser->eof()) {
//   absl::StatusOr<std::unique_ptr<SamEntry>> entry_or = parser->Next();
//   if (!entry_or.ok()) {
//     // Handle error
//   }
//   std::unique_ptr<SamEntry> entry = std::move(entry_or.value());
//   // Do stuff with entry.
// }
// ```
//
// Or if using status macros:
//
// ```cpp
// ASSIGN_OR_RETURN(std::unqiue_ptr<SamParser> parser,
//                  SamParser::New("path/to/file.sam"));
// while (!parser->eof()) {
//   ASSIGN_OR_RETURN(std::unique_ptr<SamEntry> entry, parser->Next());
//   // Do stuff with entry.
// }
// ```
class SamParser : public LineParserBase {
 public:
  explicit SamParser(absl::Nonnull<gxl::File*> file)
      : LineParserBase(file), line_number_(0) {}

  ~SamParser() = default;

  // Constructs a new SamParser from the specified file path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<SamParser>>;

  // Constructs a new SamParser from the specified file path or terminates the
  // program if constructing the parser fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<SamParser>;

  // Returns the next SAM entry from the file.
  auto Next() -> absl::StatusOr<std::unique_ptr<SamEntry>>;

  // Reads a vector of a entries in the file.
  auto All() -> absl::StatusOr<std::vector<std::unique_ptr<SamEntry>>>;

 private:
  size_t line_number_;
};

}  // namespace bio

#endif  // BIO_SAM_SAM_PARSER_H_
