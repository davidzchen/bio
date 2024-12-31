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
