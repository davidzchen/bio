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

#include "bio/sam/cigar-parser.h"

#include <vector>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/sam/cigar.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::ElementsAre;
using ::testing::HasSubstr;

TEST(CigarParser, ParseEmpty) {
  CigarParser parser;
  absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("");
  EXPECT_THAT(ops.status(), IsOk());
  EXPECT_TRUE(ops->empty());
}

TEST(CigarParser, ParseInvalidConsecutiveOps) {
  CigarParser parser;
  {
    absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("M");
    EXPECT_THAT(ops.status(),
                StatusIs(absl::StatusCode::kInvalidArgument,
                         HasSubstr("consecutive operations found")));
  }
  {
    absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("MM");
    EXPECT_THAT(ops.status(),
                StatusIs(absl::StatusCode::kInvalidArgument,
                         HasSubstr("consecutive operations found")));
  }
  {
    absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("12MM");
    EXPECT_THAT(ops.status(),
                StatusIs(absl::StatusCode::kInvalidArgument,
                         HasSubstr("consecutive operations found")));
  }
  {
    absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("1I2MM3D");
    EXPECT_THAT(ops.status(),
                StatusIs(absl::StatusCode::kInvalidArgument,
                         HasSubstr("consecutive operations found")));
  }
}

TEST(CigarParser, Parse) {
  CigarParser parser;
  {
    absl::StatusOr<std::vector<CigarOperation>> ops = parser.Parse("3M2I3M");
    EXPECT_THAT(ops.status(), IsOk());
    EXPECT_THAT(
        *ops,
        ElementsAre(
            CigarOperation{.type = CigarType::kAlignmentMatch, .length = 3},
            CigarOperation{.type = CigarType::kInsertion, .length = 2},
            CigarOperation{.type = CigarType::kAlignmentMatch, .length = 3}));
  }
  {
    absl::StatusOr<std::vector<CigarOperation>> ops =
        parser.Parse("3M20I30D200N3S4H99P10=11X");
    EXPECT_THAT(ops.status(), IsOk());
    EXPECT_THAT(
        *ops,
        ElementsAre(
            CigarOperation{.type = CigarType::kAlignmentMatch, .length = 3},
            CigarOperation{.type = CigarType::kInsertion, .length = 20},
            CigarOperation{.type = CigarType::kDeletion, .length = 30},
            CigarOperation{.type = CigarType::kSkippedRegion, .length = 200},
            CigarOperation{.type = CigarType::kSoftClipping, .length = 3},
            CigarOperation{.type = CigarType::kHardClipping, .length = 4},
            CigarOperation{.type = CigarType::kPadding, .length = 99},
            CigarOperation{.type = CigarType::kSequenceMatch, .length = 10},
            CigarOperation{.type = CigarType::kSequenceMismatch,
                           .length = 11}));
  }
}

}  // namespace
}  // namespace bio
