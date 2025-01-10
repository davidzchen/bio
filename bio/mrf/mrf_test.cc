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

#include "bio/mrf/mrf.h"

#include <memory>
#include <string>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mrf.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::HasSubstr;

TEST(MrfHeader, ParseSingleColumn) {
  const std::string line = "AlignmentBlocks";
  absl::StatusOr<std::unique_ptr<MrfHeader>> header = MrfHeader::Parse(line);
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_TRUE((*header)->HasColumn(MrfColumn::kAlignmentBlocks));
}

TEST(MrfHeader, ParseAllColumns) {
  const std::string line = "AlignmentBlocks\tSequence\tQualityScores\tQueryId";
  absl::StatusOr<std::unique_ptr<MrfHeader>> header = MrfHeader::Parse(line);
  EXPECT_THAT(header.status(), IsOk());
  EXPECT_TRUE((*header)->HasColumn(MrfColumn::kAlignmentBlocks));
  EXPECT_TRUE((*header)->HasColumn(MrfColumn::kSequence));
  EXPECT_TRUE((*header)->HasColumn(MrfColumn::kQualityScores));
  EXPECT_TRUE((*header)->HasColumn(MrfColumn::kQueryId));
  EXPECT_THAT((*header)->columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                          MrfColumn::kQualityScores, MrfColumn::kQueryId));
}

TEST(MrfHeader, ParseMissingRequiredColumn) {
  const std::string line = "Sequence\tQualityScores\tQueryId";
  absl::StatusOr<std::unique_ptr<MrfHeader>> header = MrfHeader::Parse(line);
  EXPECT_THAT(
      header.status(),
      StatusIs(absl::StatusCode::kInvalidArgument,
               HasSubstr("Required AlignmentBlocks column not present")));
}

TEST(MrfHeader, ParseInvalidColumn) {
  const std::string line = "AlignmentBlocks\tSequence\tQualityScores\tInvalid";
  absl::StatusOr<std::unique_ptr<MrfHeader>> header = MrfHeader::Parse(line);
  EXPECT_THAT(header.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                        HasSubstr("Invalid column name")));
}

TEST(MrfHeader, ParseDuplicateColumn) {
  const std::string line =
      "AlignmentBlocks\tSequence\tQualityScores\tQualityScores";
  absl::StatusOr<std::unique_ptr<MrfHeader>> header = MrfHeader::Parse(line);
  EXPECT_THAT(header.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                        HasSubstr("Duplicate column")));
}

TEST(MrfHeader, AddComments) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence});
  EXPECT_TRUE(header.comments().empty());

  const std::vector<std::string> comments = {"Comment 1", "Comment 2"};
  header.AddComments(comments);
  EXPECT_THAT(header.comments(), ElementsAreArray(comments));
}

TEST(MrfHeader, AddColumn) {
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  EXPECT_TRUE(header.HasColumn(MrfColumn::kAlignmentBlocks));
  EXPECT_FALSE(header.HasColumn(MrfColumn::kSequence));

  header.AddColumn(MrfColumn::kSequence);
  EXPECT_TRUE(header.HasColumn(MrfColumn::kSequence));
  EXPECT_THAT(header.columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kSequence));

  header.AddColumn(MrfColumn::kSequence);
  EXPECT_THAT(header.columns(),
              ElementsAre(MrfColumn::kAlignmentBlocks, MrfColumn::kSequence));
}

TEST(MrfHeader, String) {
  {
    MrfHeader header({});
    EXPECT_EQ(header.string(), "");
  }
  {
    MrfHeader header({MrfColumn::kAlignmentBlocks});
    EXPECT_EQ(header.string(), "AlignmentBlocks");
  }
  {
    MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence});
    EXPECT_EQ(header.string(), "AlignmentBlocks\tSequence");
  }
  {
    MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                      MrfColumn::kQualityScores, MrfColumn::kQueryId});
    EXPECT_EQ(header.string(),
              "AlignmentBlocks\tSequence\tQualityScores\tQueryId");
  }
}

TEST(MrfBlock, String) {
  MrfBlock empty = {};
  EXPECT_EQ(empty.string(), ":+:0:0:0:0");

  MrfBlock valid_sense = {
      .target_name = "chr4",
      .strand = Strand::kSense,
      .target_start = 1000,
      .target_end = 2000,
      .query_start = 10,
      .query_end = 20,
  };
  EXPECT_EQ(valid_sense.string(), "chr4:+:1000:2000:10:20");

  MrfBlock valid_antisense = {
      .target_name = "chr4",
      .strand = Strand::kAntisense,
      .target_start = 1000,
      .target_end = 2000,
      .query_start = 10,
      .query_end = 20,
  };
  EXPECT_EQ(valid_antisense.string(), "chr4:-:1000:2000:10:20");
}

TEST(MrfRead, Length) {
  MrfRead empty;
  EXPECT_EQ(empty.Length(), 0);

  MrfRead valid = {.blocks = {
                       {.target_start = 0, .target_end = 1000},
                       {.target_start = 2000, .target_end = 3000},
                   }};
  EXPECT_EQ(valid.Length(), 2002);
}

TEST(MrfRead, AlignmentBlocksStrSingleBlock) {
  MrfRead read = {
      .blocks = {{.target_name = "chr4",
                  .strand = Strand::kAntisense,
                  .target_start = 1221,
                  .target_end = 1270,
                  .query_start = 1,
                  .query_end = 50}},

  };
  EXPECT_EQ(read.AlignmentBlocksStr(), "chr4:-:1221:1270:1:50");
}

TEST(MrfRead, AlignmentBlocksStrMultipleBlocks) {
  MrfRead read = {
      .blocks = {{.target_name = "chr4",
                  .strand = Strand::kAntisense,
                  .target_start = 1221,
                  .target_end = 1270,
                  .query_start = 1,
                  .query_end = 50},
                 {.target_name = "chr4",
                  .strand = Strand::kSense,
                  .target_start = 2221,
                  .target_end = 2270,
                  .query_start = 1,
                  .query_end = 50},
                 {.target_name = "chr4",
                  .strand = Strand::kAntisense,
                  .target_start = 3221,
                  .target_end = 3270,
                  .query_start = 1,
                  .query_end = 50}},

  };
  EXPECT_EQ(
      read.AlignmentBlocksStr(),
      "chr4:-:1221:1270:1:50,chr4:+:2221:2270:1:50,chr4:-:3221:3270:1:50");
}

TEST(MrfEntry, ValidateSingleEndRequriedField) {
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  MrfEntry entry = {
      .is_paired_end = false,
      .read1 =
          {
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kAntisense,
                          .target_start = 1221,
                          .target_end = 1270,
                          .query_start = 1,
                          .query_end = 50}},
          },
  };
  EXPECT_THAT(entry.Validate(header), IsOk());
}

TEST(MrfEntry, ValidateSingleEndAllFields) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
  EXPECT_THAT(entry.Validate(header), IsOk());
}

TEST(MrfEntry, ValidatePairedEndRequiredField) {
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  MrfEntry entry = {
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
  EXPECT_THAT(entry.Validate(header), IsOk());
}

TEST(MrfEntry, ValidatePairedEndAllFields) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
  EXPECT_THAT(entry.Validate(header), IsOk());
}

TEST(MrfEntry, ValidateSingleEndNoBlocks) {
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  MrfEntry entry = {
      .is_paired_end = false,
      .read1 = {.blocks = {}},
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Read must have one or more blocks")));
}

TEST(MrfEntry, ValidateSingleEndBlockMissingTargetName) {
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  MrfEntry entry = {
      .is_paired_end = false,
      .read1 =
          {
              .blocks = {{.target_name = "",
                          .strand = Strand::kAntisense,
                          .target_start = 1221,
                          .target_end = 1270,
                          .query_start = 1,
                          .query_end = 50}},
          },
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("missing target name")));
}

TEST(MrfEntry, ValidateSingleEndMissingSequence) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
      .is_paired_end = false,
      .read1 =
          {
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kAntisense,
                          .target_start = 1221,
                          .target_end = 1270,
                          .query_start = 1,
                          .query_end = 50}},
              .quality_scores = "IIIIIII",
              .query_id = "1",
          },
  };
  EXPECT_THAT(
      entry.Validate(header),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          HasSubstr("Header contains Sequence column but sequence is empty")));
}

TEST(MrfEntry, ValidateSingleEndMissingQualityScores) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .query_id = "1",
          },
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Header contains QualityScores column but "
                                 "quality_scores is empty")));
}

TEST(MrfEntry, ValidateSingleEndMissingQueryId) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
          },
  };
  EXPECT_THAT(
      entry.Validate(header),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          HasSubstr("Header contains QueryId column but query_id is empty")));
}

TEST(MrfEntry, ValidatePairedEndNoBlocks) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .blocks = {},
              .sequence = "GCTA",
              .quality_scores = "JJJJ",
              .query_id = "2",
          },
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Read must have one or more blocks")));
}

TEST(MrfEntry, ValidatePairedEndMissingTargetName) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .blocks = {{.target_name = "",
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
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("missing target name")));
}

TEST(MrfEntry, ValidatePairedEndMissingSequence) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kSense,
                          .target_start = 1401,
                          .target_end = 1450,
                          .query_start = 1,
                          .query_end = 50}},
              .quality_scores = "JJJJ",
              .query_id = "2",
          },
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Header contains Sequence column but "
                                 "sequence is empty")));
}

TEST(MrfEntry, ValidatePairedEndMissingQualityScores) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kSense,
                          .target_start = 1401,
                          .target_end = 1450,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "GCTA",
              .query_id = "2",
          },
  };
  EXPECT_THAT(entry.Validate(header),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Header contains QualityScores column but "
                                 "quality_scores is empty")));
}

TEST(MrfEntry, ValidatePairedEndMissingQueryId) {
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  MrfEntry entry = {
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
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kSense,
                          .target_start = 1401,
                          .target_end = 1450,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "GCTA",
              .quality_scores = "JJJJ",
          },
  };
  EXPECT_THAT(
      entry.Validate(header),
      StatusIs(
          absl::StatusCode::kInvalidArgument,
          HasSubstr("Header contains QueryId column but query_id is empty")));
}

}  // namespace
}  // namespace bio
