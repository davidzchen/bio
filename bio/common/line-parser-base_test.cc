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

#include <cstdint>
#include <optional>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "bio/common/sequence.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::IsOkAndHolds;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

class LineParserPeer : public LineParserBase {
 public:
  explicit LineParserPeer(absl::Nonnull<gxl::File*> file)
      : LineParserBase(file) {}

  using LineParserBase::NextLine;
  using LineParserBase::ParseDouble;
  using LineParserBase::ParseInt;
  using LineParserBase::ParseStrand;
  using LineParserBase::ParseUInt8;
  using LineParserBase::PutBack;
};

TEST(LineParserBase, NextLineEmptyFile) {
  gxl::File* file;
  absl::Status status =
      gxl::Open("bio/common/testdata/empty", "r", &file, gxl::file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, std::nullopt);
}

TEST(LineParserBase, NextLine) {
  gxl::File* file;
  absl::Status status =
      gxl::Open("bio/common/testdata/lines", "r", &file, gxl::file::Defaults());
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
      gxl::Open("bio/common/testdata/lines", "r", &file, gxl::file::Defaults());
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

TEST(LineParserBase, ParseIntUInt) {
  gxl::File* file;
  absl::Status status = gxl::Open("bio/common/testdata/unused", "r", &file,
                                  gxl::file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);

  // uint64_t
  EXPECT_THAT(parser.ParseInt<uint64_t>("0", "foo_field"), IsOkAndHolds(0));
  EXPECT_THAT(parser.ParseInt<uint64_t>("18446744073709551615", "foo_field"),
              IsOkAndHolds(static_cast<uint64_t>(18446744073709551615)));
  EXPECT_THAT(parser.ParseInt<uint64_t>("-1", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
  EXPECT_THAT(parser.ParseInt<uint64_t>("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));

  // int64_t
  EXPECT_THAT(parser.ParseInt<int64_t>("0", "foo_field"), IsOkAndHolds(0));
  EXPECT_THAT(parser.ParseInt<int64_t>("9223372036854775807", "foo_field"),
              IsOkAndHolds(9223372036854775807));
  EXPECT_THAT(parser.ParseInt<int64_t>("-9223372036854775807", "foo_field"),
              IsOkAndHolds(-9223372036854775807));
  EXPECT_THAT(parser.ParseInt<int64_t>("9223372036854775808", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
  EXPECT_THAT(parser.ParseInt<uint64_t>("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));

  // uint32_t
  EXPECT_THAT(parser.ParseInt<uint32_t>("0", "foo_field"), IsOkAndHolds(0));
  EXPECT_THAT(parser.ParseInt<uint32_t>("4294967295", "foo_field"),
              IsOkAndHolds(4294967295));
  EXPECT_THAT(parser.ParseInt<uint32_t>("4294967296", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
  EXPECT_THAT(parser.ParseInt<uint32_t>("-1", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
  EXPECT_THAT(parser.ParseInt<uint64_t>("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));

  // int32_t
  EXPECT_THAT(parser.ParseInt<int32_t>("0", "foo_field"), IsOkAndHolds(0));
  EXPECT_THAT(parser.ParseInt<int32_t>("2147483647", "foo_field"),
              IsOkAndHolds(2147483647));
  EXPECT_THAT(parser.ParseInt<int32_t>("-2147483648", "foo_field"),
              IsOkAndHolds(-2147483648));
  EXPECT_THAT(parser.ParseInt<int32_t>("2147483649", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
  EXPECT_THAT(parser.ParseInt<uint32_t>("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
}

TEST(LineParserBase, ParseUInt8) {
  gxl::File* file;
  absl::Status status = gxl::Open("bio/common/testdata/unused", "r", &file,
                                  gxl::file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  EXPECT_THAT(parser.ParseUInt8("0", "foo_field"), IsOkAndHolds(0));
  EXPECT_THAT(parser.ParseUInt8("255", "foo_field"), IsOkAndHolds(255));
  EXPECT_THAT(parser.ParseUInt8("-1", "foo_field"),
              StatusIs(absl::StatusCode::kOutOfRange,
                       HasSubstr("value is out of range for a uint8_t")));
  EXPECT_THAT(parser.ParseUInt8("256", "foo_field"),
              StatusIs(absl::StatusCode::kOutOfRange,
                       HasSubstr("value is out of range for a uint8_t")));
  EXPECT_THAT(parser.ParseUInt8("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Failed to parse")));
}

TEST(LineParseBase, ParseDouble) {
  gxl::File* file;
  absl::Status status = gxl::Open("bio/common/testdata/unused", "r", &file,
                                  gxl::file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  EXPECT_THAT(parser.ParseDouble("0", "foo_field"), IsOkAndHolds(0.0));
  EXPECT_THAT(parser.ParseDouble("invalid", "foo_field"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid foo_field format")));
}

TEST(LineParserBase, ParseStrand) {
  gxl::File* file;
  absl::Status status = gxl::Open("bio/common/testdata/unused", "r", &file,
                                  gxl::file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  EXPECT_THAT(parser.ParseStrand("+"), IsOkAndHolds(Strand::kSense));
  EXPECT_THAT(parser.ParseStrand("-"), IsOkAndHolds(Strand::kAntisense));
  EXPECT_THAT(parser.ParseStrand("invalid"),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid strand format")));
}

}  // namespace
}  // namespace bio
