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

#include "bio/mrf/mrf-parser.h"

#include <memory>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/common/sequence.h"
#include "bio/mrf/mrf.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::ElementsAre;
using ::testing::HasSubstr;

TEST(MrfParser, NextEmpty) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/empty.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_EQ(*header, nullptr);
  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_EQ(*entry, nullptr);
  EXPECT_TRUE(parser->eof());
}

TEST(MrfParser, NextCalledBeforeStart) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/single-alignment-block.mrf");

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kFailedPrecondition,
                       HasSubstr("Start() must be called before Next()")));
}

TEST(MrfParser, NextNumColumnsNotMatching) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-num-fields-differ.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("number of columns do not match header")));
}

TEST(MrfParser, NextInvalidBlockNumFields) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-block-num-fields.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid number of fields for block")));
}

TEST(MrfParser, NextInvalidTargetStart) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-target-start.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid target start format")));
}

TEST(MrfParser, NextInvalidTargetEnd) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-target-end.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid target end format")));
}

TEST(MrfParser, NextInvalidQueryStart) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-query-start.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Invalid query start format")));
}

TEST(MrfParser, NextInvalidQueryEnd) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-query-end.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid query end format")));
}

TEST(MrfParser, NextInvalidStrand) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/invalid-strand.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());

  absl::StatusOr<std::unique_ptr<MrfEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid strand format")));
}

auto CheckMrfReadEquals(const MrfRead& actual, const MrfRead& expected) {
  EXPECT_EQ(actual.blocks.size(), expected.blocks.size());
  for (int i = 0; i < actual.blocks.size(); ++i) {
    const MrfBlock& actual_block = actual.blocks[i];
    const MrfBlock& expected_block = expected.blocks[i];
    EXPECT_EQ(actual_block.target_name, expected_block.target_name);
    EXPECT_EQ(actual_block.strand, expected_block.strand);
    EXPECT_EQ(actual_block.target_start, expected_block.target_start);
    EXPECT_EQ(actual_block.target_end, expected_block.target_end);
    EXPECT_EQ(actual_block.query_start, expected_block.query_start);
    EXPECT_EQ(actual_block.query_end, expected_block.query_end);
  }
  EXPECT_EQ(actual.sequence, expected.sequence);
  EXPECT_EQ(actual.quality_scores, expected.quality_scores);
  EXPECT_EQ(actual.query_id, expected.query_id);
}

auto CheckMrfEntryEquals(const MrfEntry* actual, const MrfEntry& expected) {
  EXPECT_EQ(actual->is_paired_end, expected.is_paired_end);
  CheckMrfReadEquals(actual->read1, expected.read1);
  if (actual->is_paired_end) {
    CheckMrfReadEquals(actual->read2, expected.read2);
  }
}

TEST(MrfParser, NextSingleAlignmentBlock) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/single-alignment-block.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_THAT((*header)->columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kQueryId));
  EXPECT_THAT((*header)->comments(), ElementsAre("Example 2"));

  {
    absl::StatusOr<std::unique_ptr<MrfEntry>> actual = parser->Next();
    EXPECT_THAT(actual.status(), IsOk());
    MrfEntry expected = {
        .is_paired_end = false,
        .read1 = {.blocks = {{.target_name = "chr4",
                              .strand = Strand::kAntisense,
                              .target_start = 1221,
                              .target_end = 1270,
                              .query_start = 1,
                              .query_end = 50}},
                  .query_id = "1"},
    };
    CheckMrfEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<MrfEntry>> actual = parser->Next();
    EXPECT_THAT(actual.status(), IsOk());
    MrfEntry expected = {
        .is_paired_end = false,
        .read1 = {.blocks = {{.target_name = "chr16",
                              .strand = Strand::kSense,
                              .target_start = 511,
                              .target_end = 560,
                              .query_start = 1,
                              .query_end = 50}},
                  .query_id = "2"},
    };
    CheckMrfEntryEquals(actual->get(), expected);
  }
}

TEST(MrfParser, NextMultipleAlignmentBlocks) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/multiple-alignment-blocks.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_THAT((*header)->columns(), ElementsAre(MrfColumn::kAlignmentBlocks));
  EXPECT_THAT((*header)->comments(), ElementsAre("Example 1"));

  absl::StatusOr<std::unique_ptr<MrfEntry>> actual = parser->Next();
  EXPECT_THAT(actual.status(), IsOk());
  MrfEntry expected = {
      .is_paired_end = true,
      .read1 = {.blocks = {{.target_name = "chr2",
                            .strand = Strand::kSense,
                            .target_start = 601,
                            .target_end = 630,
                            .query_start = 1,
                            .query_end = 30},
                           {.target_name = "chr2",
                            .strand = Strand::kSense,
                            .target_start = 921,
                            .target_end = 940,
                            .query_start = 31,
                            .query_end = 50}}},
      .read2 = {.blocks = {{
                    .target_name = "chr2",
                    .strand = Strand::kSense,
                    .target_start = 1401,
                    .target_end = 1450,
                    .query_start = 1,
                    .query_end = 50,
                }}},
  };
  CheckMrfEntryEquals(actual->get(), expected);
}

TEST(MrfParser, NextAllFieldsSingleRead) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/all-fields-single-read.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_THAT((*header)->columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                          MrfColumn::kQualityScores, MrfColumn::kQueryId));
  EXPECT_THAT((*header)->comments(), ElementsAre("Example 2"));

  absl::StatusOr<std::unique_ptr<MrfEntry>> actual = parser->Next();
  EXPECT_THAT(actual.status(), IsOk());
  MrfEntry expected = {
      .is_paired_end = false,
      .read1 =
          {
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kAntisense,
                          .target_start = 1221,
                          .target_end = 1270,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "ACTCGAA",
              .quality_scores = "IIIIIII",
              .query_id = "1",
          },
  };
  CheckMrfEntryEquals(actual->get(), expected);
}

TEST(MrfParser, NextAllFieldsPairedEnd) {
  std::unique_ptr<MrfParser> parser =
      MrfParser::NewOrDie("bio/mrf/testdata/all-fields-paired-end.mrf");

  absl::StatusOr<std::unique_ptr<MrfHeader>> header = parser->Start();
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_THAT((*header)->columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                          MrfColumn::kQualityScores, MrfColumn::kQueryId));
  EXPECT_THAT((*header)->comments(),
              ElementsAre("Paired ends with all fields"));

  absl::StatusOr<std::unique_ptr<MrfEntry>> actual = parser->Next();
  EXPECT_THAT(actual.status(), IsOk());
  MrfEntry expected = {
      .is_paired_end = true,
      .read1 =
          {
              .blocks = {{.target_name = "chr2",
                          .strand = Strand::kSense,
                          .target_start = 601,
                          .target_end = 630,
                          .query_start = 1,
                          .query_end = 30}},
              .sequence = "ACTG",
              .quality_scores = "IIII",
              .query_id = "1",
          },
      .read2 =
          {
              .blocks = {{.target_name = "chr2",
                          .strand = Strand::kSense,
                          .target_start = 1401,
                          .target_end = 1450,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "GCTA",
              .quality_scores = "JJJJ",
              .query_id = "2",
          },
  };
  CheckMrfEntryEquals(actual->get(), expected);
}

}  // namespace
}  // namespace bio
