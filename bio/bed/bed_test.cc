#include "bio/bed/bed.h"

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(BedEntry, StringBed3) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000");
}

TEST(BedEntry, StringBed4) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest");
}

TEST(BedEntry, StringBed5) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest\t900");
}

TEST(BedEntry, StringBed6Sense) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
      .strand = BedStrand::kSense,
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest\t900\t+");
}

TEST(BedEntry, StringBed6Antisense) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
      .strand = BedStrand::kAntisense,
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest\t900\t-");
}

TEST(BedEntry, StringBed8) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
      .strand = BedStrand::kSense,
      .thick_start = 50,
      .thick_end = 1000,
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest\t900\t+\t50\t1000");
}

TEST(BedEntry, StringBed9) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
      .strand = BedStrand::kSense,
      .thick_start = 50,
      .thick_end = 1000,
      .item_rgb = "255,0,0",
  };
  EXPECT_EQ(entry.string(), "chr7\t0\t1000\tTest\t900\t+\t50\t1000\t255,0,0");
}

TEST(BedEntry, StringBed12) {
  BedEntry entry = {
      .chromosome = "chr7",
      .start = 0,
      .end = 1000,
      .name = "Test",
      .score = 900,
      .strand = BedStrand::kSense,
      .thick_start = 50,
      .thick_end = 1000,
      .item_rgb = "255,0,0",
      .sub_blocks = {{.size = 567, .start = 0}, {.size = 488, .start = 3512}},
  };
  EXPECT_EQ(
      entry.string(),
      "chr7\t0\t1000\tTest\t900\t+\t50\t1000\t255,0,0\t2\t567,488\t0,3512");
}

}  // namespace
}  // namespace bio
