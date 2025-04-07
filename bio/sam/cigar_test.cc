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

#include "bio/sam/cigar.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(Cigar, ToStringEmpty) {
  Cigar cigar;
  EXPECT_EQ(cigar.string(), "");
}

TEST(Cigar, ToString) {
  Cigar cigar = {
      .operations = {{CigarType::kAlignmentMatch, 3},
                     {CigarType::kInsertion, 2},
                     {CigarType::kAlignmentMatch, 3}},
  };
  EXPECT_EQ(cigar.string(), "3M2I3M");
}

}  // namespace
}  // namespace bio
