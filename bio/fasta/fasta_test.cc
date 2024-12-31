#include "bio/fasta/fasta.h"

#include <string>

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FastaSequence, StringEmptySequence) {
  FastaSequence sequence = {
      .name = "SEQUENCE",
      .sequence = "",
  };
  EXPECT_EQ(sequence.string(), ">SEQUENCE\n\n");
}

TEST(FastaSequence, String) {
  FastaSequence sequence = {
      .name = "MCHU",
      .sequence =
          "MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADGNGTIDFPE"
          "FLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDEEVDEMIREADIDGDGQ"
          "VNYEEFVQMMTAK*",
  };
  const std::string expected = R"(>MCHU
MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADG
NGTIDFPEFLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDE
EVDEMIREADIDGDGQVNYEEFVQMMTAK*
)";
  EXPECT_EQ(sequence.string(), expected);
}

}  // namespace
}  // namespace bio
