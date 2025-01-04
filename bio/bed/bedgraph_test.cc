#include "bio/bed/bedgraph.h"

#include "gtest/gtest.h"

namespace bio {

TEST(BedGraphEntry, Correctness) {
  BedGraphEntry entry = {
      .chromosome = "chr7",
      .start = 1000,
      .end = 2000,
      .value = -1.0,
  };
  EXPECT_EQ(entry.string(), "chr7\t1000\t2000\t-1.00");
}

}  // namespace bio
