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

#include "bio/bed/bed-parser.h"

#include <memory>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/bed/bed.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(BedParser, NextEntryEmpty) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/empty.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_EQ(*entry, nullptr);
  EXPECT_TRUE(parser->eof());
}

TEST(BedParser, NextEntryInvalidMissingChromStart) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-missing-required-fields-1.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected at least 3 fields but got only")));
}

TEST(BedParser, NextEntryInvalidMissingChromEnd) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-missing-required-fields-2.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected at least 3 fields but got only")));
}

TEST(BedParser, NextEntryInvalidChromStart) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-chrom-start.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid feature start format")));
}

TEST(BedParser, NextEntryInvalidChromEnd) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-chrom-end.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid feature end format")));
}

TEST(BedParser, NextEntryInvalidScore) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-score.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid score format")));
}

TEST(BedParser, NextEntryInvalidStrand) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-strand.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid strand format")));
}

TEST(BedParser, NextEntryInvalidThickStart) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-thick-start.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid thick start format")));
}

TEST(BedParser, NextEntryInvalidThickEnd) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-thick-end.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid thick end format")));
}

TEST(BedParser, NextEntryBed12MissingBlockSizesAndStarts) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-missing-fields-1.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected 12 fields but got")));
}

TEST(BedParser, NextEntryBed12MissingBlockStarts) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-missing-fields-2.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected 12 fields but got")));
}

TEST(BedParser, NextEntryBed12BlockSizesNotMatching) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-block-sizes-not-matching.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("number of block sizes")));
}

TEST(BedParser, NextEntryBed12BlockStartsNotMatching) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-block-starts-not-matching.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("number of block starts")));
}

TEST(BedParser, NextEntryBed12InvalidBlockSize) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-block-size-format.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("invalid block size format")));
}

TEST(BedParser, NextEntryBed12InvalidBlockStart) {
  std::unique_ptr<BedParser> parser = BedParser::NewOrDie(
      "bio/bed/testdata/invalid-bed12-block-start-format.bed");

  absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("invalid block start format")));
}

TEST(BedParser, NextEntryNumFieldsDiffer) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/invalid-num-fields-differ.bed");

  {
    absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
    EXPECT_THAT(entry.status(), IsOk());
  }
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> entry = parser->NextEntry();
    EXPECT_THAT(entry.status(),
                StatusIs(absl::StatusCode::kInvalidArgument,
                         HasSubstr("Expected 3 fields but got")));
  }
}

auto CheckEntryEquals(const BedEntry* actual, const BedEntry& expected)
    -> void {
  EXPECT_EQ(actual->chromosome, expected.chromosome);
  EXPECT_EQ(actual->start, expected.start);
  EXPECT_EQ(actual->end, expected.end);

  EXPECT_EQ(actual->name.has_value(), expected.name.has_value());
  if (actual->name.has_value()) {
    EXPECT_EQ(actual->name, expected.name);
  }

  EXPECT_EQ(actual->score.has_value(), expected.score.has_value());
  if (actual->score.has_value()) {
    EXPECT_EQ(actual->score, expected.score);
  }

  EXPECT_EQ(actual->strand.has_value(), expected.strand.has_value());
  if (actual->strand.has_value()) {
    EXPECT_EQ(actual->strand, expected.strand);
  }

  EXPECT_EQ(actual->thick_start.has_value(), expected.thick_start.has_value());
  if (actual->thick_start.has_value()) {
    EXPECT_EQ(actual->thick_start, expected.thick_start);
  }

  EXPECT_EQ(actual->thick_end.has_value(), expected.thick_end.has_value());
  if (actual->thick_end.has_value()) {
    EXPECT_EQ(actual->thick_end, expected.thick_end);
  }

  EXPECT_EQ(actual->item_rgb.has_value(), expected.item_rgb.has_value());
  if (actual->item_rgb.has_value()) {
    EXPECT_EQ(actual->item_rgb, expected.item_rgb);
  }

  EXPECT_EQ(actual->sub_blocks.size(), expected.sub_blocks.size());
  for (int i = 0; i < actual->sub_blocks.size(); ++i) {
    EXPECT_EQ(actual->sub_blocks[i].start, expected.sub_blocks[i].start);
    EXPECT_EQ(actual->sub_blocks[i].size, expected.sub_blocks[i].size);
  }
}

TEST(BedParser, NextEntryBed3) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/bed3.bed");
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127471196,
        .end = 127472363,
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127472363,
        .end = 127473530,
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

TEST(BedParser, NextEntryBed3WithComments) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/bed3-with-comments.bed");
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127471196,
        .end = 127472363,
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127472363,
        .end = 127473530,
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

TEST(BedParser, NextEntryBed6GenomeBrowser) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/bed6-genome-browser.bed");
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127471196,
        .end = 127472363,
        .name = "Pos1",
        .score = 0,
        .strand = BedStrand::kSense,
        .thick_start = 127471196,
        .thick_end = 127472363,
        .item_rgb = "255,0,0",
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr7",
        .start = 127475864,
        .end = 127477031,
        .name = "Neg1",
        .score = 0,
        .strand = BedStrand::kAntisense,
        .thick_start = 127475864,
        .thick_end = 127477031,
        .item_rgb = "0,0,255",
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

TEST(BedParser, NextEntryBed12) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/bed12.bed");
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr22",
        .start = 1000,
        .end = 5000,
        .name = "cloneA",
        .score = 960,
        .strand = BedStrand::kSense,
        .thick_start = 1000,
        .thick_end = 5000,
        .item_rgb = "0",
        .sub_blocks = {{.size = 567, .start = 0}, {.size = 488, .start = 3512}},
    };
    CheckEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<BedEntry>> actual = parser->NextEntry();
    EXPECT_THAT(actual.status(), IsOk());
    BedEntry expected = {
        .chromosome = "chr22",
        .start = 2000,
        .end = 6000,
        .name = "cloneB",
        .score = 900,
        .strand = BedStrand::kAntisense,
        .thick_start = 2000,
        .thick_end = 6000,
        .item_rgb = "0",
        .sub_blocks = {{.size = 433, .start = 0}, {.size = 399, .start = 3601}},
    };
    CheckEntryEquals(actual->get(), expected);
  }
}

auto CheckEntriesEquals(const std::vector<std::unique_ptr<BedEntry>>& actual,
                        const std::vector<BedEntry>& expected) -> void {
  EXPECT_EQ(actual.size(), expected.size());
  for (int i = 0; i < actual.size(); ++i) {
    CheckEntryEquals(actual[i].get(), expected[i]);
  }
}

TEST(BedParser, AllENtries) {
  std::unique_ptr<BedParser> parser =
      BedParser::NewOrDie("bio/bed/testdata/bed12.bed");
  absl::StatusOr<std::vector<std::unique_ptr<BedEntry>>> actual =
      parser->AllEntries();

  std::vector<BedEntry> expected = {
      {
          .chromosome = "chr22",
          .start = 1000,
          .end = 5000,
          .name = "cloneA",
          .score = 960,
          .strand = BedStrand::kSense,
          .thick_start = 1000,
          .thick_end = 5000,
          .item_rgb = "0",
          .sub_blocks = {{.size = 567, .start = 0},
                         {.size = 488, .start = 3512}},
      },
      {
          .chromosome = "chr22",
          .start = 2000,
          .end = 6000,
          .name = "cloneB",
          .score = 900,
          .strand = BedStrand::kAntisense,
          .thick_start = 2000,
          .thick_end = 6000,
          .item_rgb = "0",
          .sub_blocks = {{.size = 433, .start = 0},
                         {.size = 399, .start = 3601}},
      }};
  CheckEntriesEquals(*actual, expected);
}

}  // namespace
}  // namespace bio
