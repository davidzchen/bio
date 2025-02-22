#include "bio/common/word-iterator.h"

#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::testing::Optional;

TEST(WordIterator, NextIsEmpty) {
  WordIterator it("", "");
  EXPECT_FALSE(it.Next().has_value());
}

TEST(WordIterator, NextIsEmptyWithSeparators) {
  WordIterator it("", "012");
  EXPECT_FALSE(it.Next().has_value());
}

TEST(WordIterator, Next) {
  WordIterator it("AA0BB1CC2DD", "012");
  EXPECT_THAT(it.Next(), Optional(absl::string_view("AA")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("BB")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("CC")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("DD")));
  EXPECT_FALSE(it.Next().has_value());
}

TEST(WordIterator, NextWithEmptyStrings) {
  WordIterator it("AA0BB12CC", "012");
  EXPECT_THAT(it.Next(), Optional(absl::string_view("AA")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("BB")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("")));
  EXPECT_THAT(it.Next(), Optional(absl::string_view("CC")));
  EXPECT_FALSE(it.Next().has_value());
}

}  // namespace
}  // namespace bio
