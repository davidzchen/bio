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

#ifndef BIO_BED_BED_PARSER_H_
#define BIO_BED_BED_PARSER_H_

#include <cstdlib>
#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "bio/bed/bed.h"
#include "bio/common/line-parser-base.h"

namespace bio {

class BedParser : public LineParserBase {
 public:
  explicit BedParser(absl::Nonnull<abxl::File*> file)
      : LineParserBase(file), line_number_(0), num_fields_(0) {}

  ~BedParser() = default;

  // Constructs a new BedParser from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<BedParser>>;

  // Constructs a new BedParser from the specified file path or terminates the
  // program if constructing the parser fails.
  static auto NewOrDie(absl::string_view path) -> std::unique_ptr<BedParser>;

  // Returns the next BED entry from the file.
  auto NextEntry() -> absl::StatusOr<std::unique_ptr<BedEntry>>;

  // Returns a vector of all entries from the file.
  auto AllEntries() -> absl::StatusOr<std::vector<std::unique_ptr<BedEntry>>>;

 private:
  // Used to track line numbers.
  size_t line_number_;

  // Used to track the number of BED fields used by the file and whether
  // subsequent lines contain the same number of fields.
  int num_fields_;
};

}  // namespace bio

#endif  // BIO_BED_BED_PARSER_H_
