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

}  // namespace
}  // namespace bio
