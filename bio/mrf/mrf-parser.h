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

#ifndef BIO_MRF_MRF_PARSER_H_
#define BIO_MRF_MRF_PARSER_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/common/line-parser-base.h"
#include "bio/mrf/mrf.h"

namespace bio {

// Parses a Mapped Read Format (MRF)
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<MrfParser>> parser_or =
//     MrfParser::New("path/to/file.mrf");
// if (!parser_or.status().ok()) {
//   // Handle error.
// }
// std::unique_ptr<MrfParser> parser = std::move(parser_or.value());
// absl::StatusOr<std::unique_ptr<MrfHeader>> header_or = parser->Start();
// if (!header_or.ok()) {
//   // Handle error
// }
// std::unique_ptr<MrfHeader> header = std::move(header_or.value());
// while (!parser->eof()) {
//   absl::StatusOr<std::unique_ptr<MrfEntry>> entry_or = parser->Next();
//   if (!entry_or.ok()) {
//     // Handle error.
//   }
//   std::unique_ptr<MrfEntry> entry = std::move(entry_or.value());
//   // Do stuff with entry.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<MrfParser> parser,
//                  MrfParser::New("path/to/file.mrf"));
// ASSIGN_OR_RETURN(std::unique_ptr<MrfHeader> header, parser->Start());
// while (!parser.eof()) {
//   ASSIGN_OR_RETURN(std::unique_ptr<MrfEntry> entry, parser->Next());
//   // Do stuff with entry.
// }
// ```
class MrfParser : public LineParserBase {
 public:
  explicit MrfParser(absl::Nonnull<abxl::File*> file)
      : LineParserBase(file), started_(false), line_number_(0) {}

  ~MrfParser() = default;

  // Constructs a new FastqParser from the specified file path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<MrfParser>>;

  // Constructs a new FastqParser from the specified file path or terminates the
  // program if constructing the parser fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<MrfParser>;

  // Parses and returns the MRF file's header.
  auto Start() -> absl::StatusOr<std::unique_ptr<MrfHeader>>;

  // Parses and returns the next MRF entry in the file. Start() must be called
  // before calling this method.
  auto Next() -> absl::StatusOr<std::unique_ptr<MrfEntry>>;

 private:
  std::vector<std::string> comments_;
  std::vector<MrfColumn> columns_;
  bool started_;
  size_t line_number_;
};

}  // namespace bio

#endif  // BIO_MRF_MRF_PARSER_H_
