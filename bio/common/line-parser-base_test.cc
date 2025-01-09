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

#include "bio/common/line-parser-base.h"

#include <optional>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"

namespace bio {
namespace {

namespace file = gxl::file;
using ::absl_testing::IsOk;

class LineParserPeer : public LineParserBase {
 public:
  explicit LineParserPeer(absl::Nonnull<gxl::File*> file)
      : LineParserBase(file) {}

  using LineParserBase::NextLine;
  using LineParserBase::PutBack;
};

TEST(LineParserBase, NextLineEmptyFile) {
  gxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/empty", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, std::nullopt);
}

TEST(LineParserBase, NextLine) {
  gxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/lines", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 2");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 3");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 4");

  line = parser.NextLine();
  EXPECT_EQ(line, std::nullopt);
}

TEST(LineParserBase, PutBack) {
  gxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/lines", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  parser.PutBack(*line);
  line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 2");
}

}  // namespace
}  // namespace bio
