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

#ifndef BIO_BEDGRAPH_BEDGRAPH_PARSER_H_
#define BIO_BEDGRAPH_BEDGRAPH_PARSER_H_

#include <cstdlib>
#include <memory>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/bedgraph/bedgraph.h"
#include "bio/common/line-parser-base.h"
#include "gxl/file/file.h"

namespace bio {

class BedGraphParser : public LineParserBase {
 public:
  explicit BedGraphParser(absl::Nonnull<gxl::File*> file)
      : LineParserBase(file) {}

  ~BedGraphParser() = default;

  // Constructs a new BedGraphParser from the specified path.
  static auto New(absl::string_view path)
      -> absl::StatusOr<std::unique_ptr<BedGraphParser>>;

  // Constructs a new BedGraphParser from the specified file path or terminates
  // the program if constructing the parser fails.
  static auto NewOrDie(absl::string_view path)
      -> std::unique_ptr<BedGraphParser>;

  // Returns the next BedGraph entry from the file.
  auto Next() -> absl::StatusOr<std::unique_ptr<BedGraphEntry>>;

  // Returns a vector of all entries from the file.
  auto All() -> absl::StatusOr<std::vector<std::unique_ptr<BedGraphEntry>>>;
};

}  // namespace bio

#endif  // BIO_BEDGRAPH_BEDGRAPH_PARSER_H_
