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

#include "bio/bed/bedgraph-parser.h"

#include <memory>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/bed/bedgraph.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(BedGraphParser, NextEntryEmpty) {
  std::unique_ptr<BedGraphParser> parser =
      BedGraphParser::NewOrDie("bio/bed/testdata/bedgraph/empty.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_EQ(*entry, nullptr);
  EXPECT_TRUE(parser->eof());
}

TEST(BedGraphParser, NextEntryMissingStart) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-num-fields-1.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected 3 fields but got ")));
}

TEST(BedGraphParser, NextEntryMissingEnd) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-num-fields-2.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected 3 fields but got ")));
}

TEST(BedGraphParser, NextEntryMissingValue) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-num-fields-3.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected 3 fields but got ")));
}

TEST(BedGraphParser, NextEntryInvalidStart) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-start.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid start format")));
}

TEST(BedGraphParser, NextEntryInvalidEnd) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-end.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid end format")));
}

TEST(BedGraphParser, NextEntryInvalidValue) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/invalid-value.bedgraph");

  absl::StatusOr<std::unique_ptr<BedGraphEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid value format")));
}

auto CheckEntryEquals(const BedGraphEntry* actual,
                      const BedGraphEntry& expected) -> void {
  EXPECT_EQ(actual->chromosome, expected.chromosome);
  EXPECT_EQ(actual->start, expected.start);
  EXPECT_EQ(actual->end, expected.end);
  EXPECT_NEAR(actual->value, expected.value, 0.05);
}

TEST(BedGraphParser, NextEntryValid) {
  std::unique_ptr<BedGraphParser> parser =
      BedGraphParser::NewOrDie("bio/bed/testdata/bedgraph/valid.bedgraph");

  {
    absl::StatusOr<std::unique_ptr<BedGraphEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedGraphEntry expected = {
        .chromosome = "chr19",
        .start = 49302000,
        .end = 49302300,
        .value = -1.0,
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedGraphEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedGraphEntry expected = {
        .chromosome = "chr19",
        .start = 49302300,
        .end = 49302600,
        .value = -0.75,
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

TEST(BedGraphParser, NextEntryValidGenomeBrowser) {
  std::unique_ptr<BedGraphParser> parser = BedGraphParser::NewOrDie(
      "bio/bed/testdata/bedgraph/genome-browser.bedgraph");

  {
    absl::StatusOr<std::unique_ptr<BedGraphEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedGraphEntry expected = {
        .chromosome = "chr17",
        .start = 49302000,
        .end = 49302300,
        .value = -1.0,
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedGraphEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedGraphEntry expected = {
        .chromosome = "chr17",
        .start = 49302000,
        .end = 49302300,
        .value = -1.0,
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

}  // namespace
}  // namespace bio
