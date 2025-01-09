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

TEST(MrfRead, LengthNoBlocks) {
  MrfRead read;
  EXPECT_EQ(read.Length(), 0);
}

TEST(MrfRead, Length) {
  MrfRead read = {.blocks = {
                      {.target_start = 0, .target_end = 1000},
                      {.target_start = 2000, .target_end = 3000},
                  }};
  EXPECT_EQ(read.Length(), 2002);
}

}  // namespace
}  // namespace bio
