#include "bio/fastq/fastq.h"

#include <string>

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FastqSequence, StringEmptySequence) {
  FastqSequence sequence = {
      .name = "SEQUENCE",
      .sequence = "",
      .quality = "",
  };
  EXPECT_EQ(sequence.string(), "@SEQUENCE\n\n+\n");
}

TEST(FastqSequence, String) {
  FastqSequence sequence = {
      .name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36",
      .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC",
      .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC",
  };
  EXPECT_EQ(sequence.string(),
            R"(@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36
GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC)");
}

}  // namespace
}  // namespace bio
