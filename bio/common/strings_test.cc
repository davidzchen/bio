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

#include "bio/common/strings.h"

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FirstWord, Correctness) {
  EXPECT_EQ(FirstWord(""), "");
  EXPECT_EQ(FirstWord("SEQ_ID"), "SEQ_ID");
  EXPECT_EQ(FirstWord("SEQ_ID"), "SEQ_ID");
  EXPECT_EQ(FirstWord("EAS139:136:FC706VJ:2:2104:15343:197393 1:Y:18:ATCACG"),
            "EAS139:136:FC706VJ:2:2104:15343:197393");
}

TEST(InsertWordEveryNthPosition, Correctness) {
  EXPECT_EQ(InsertWordEveryNthPosition("", "foo", 5), "");
  EXPECT_EQ(InsertWordEveryNthPosition("", "", 5), "");
  EXPECT_EQ(InsertWordEveryNthPosition("abcdefg", "", 3), "abcdefg");

  EXPECT_EQ(InsertWordEveryNthPosition("abcdefg", "0", 3), "abc0def0g");
  EXPECT_EQ(InsertWordEveryNthPosition("abcdefghi", "00", 3), "abc00def00ghi");

  EXPECT_EQ(InsertWordEveryNthPosition("abcdefghi", "00", 0), "abcdefghi");
  EXPECT_EQ(InsertWordEveryNthPosition("abcdefghi", "00", 1),
            "a00b00c00d00e00f00g00h00i");
}

}  // namespace
}  // namespace bio
