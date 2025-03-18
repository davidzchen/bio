#include "bio/sam/sam.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(SamEntry, String) {
  SamEntry entry = {
      .qname = "r003",
      .flags = 0,
      .rname = "ref",
      .pos = 9,
      .mapq = 30,
      .cigar = "5S6M",
      .rnext = "*",
      .pnext = 0,
      .tlen = 0,
  };
  EXPECT_EQ(entry.string(), "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\t*\t*");
}

TEST(SamEntry, StringWithtags) {
  SamEntry entry = {
      .qname = "r003",
      .flags = 0,
      .rname = "ref",
      .pos = 9,
      .mapq = 30,
      .cigar = "5S6M",
      .rnext = "*",
      .pnext = 0,
      .tlen = 0,
      .tags = {"SA:Z:ref,29,-,6H5M,17,0;"},
  };
  EXPECT_EQ(entry.string(),
            "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\t*\t*\tSA:Z:ref,29,-,"
            "6H5M,17,0;");
}

TEST(SamEntry, StringWithSeq) {
  SamEntry entry = {
      .qname = "r003",
      .flags = 0,
      .rname = "ref",
      .pos = 9,
      .mapq = 30,
      .cigar = "5S6M",
      .rnext = "*",
      .pnext = 0,
      .tlen = 0,
      .seq = "GCCTAAGCTAA",
      .tags = {"SA:Z:ref,29,-,6H5M,17,0;"},
  };
  EXPECT_EQ(entry.string(),
            "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\tGCCTAAGCTAA\t*\tSA:Z:ref,29,-,"
            "6H5M,17,0;");
}

TEST(SamEntry, StringWithQual) {
  SamEntry entry = {
      .qname = "r003",
      .flags = 0,
      .rname = "ref",
      .pos = 9,
      .mapq = 30,
      .cigar = "5S6M",
      .rnext = "*",
      .pnext = 0,
      .tlen = 0,
      .seq = "GCCTAAGCTAA",
      .qual = "IIIIIIIIIII",
      .tags = {"SA:Z:ref,29,-,6H5M,17,0;"},
  };
  EXPECT_EQ(entry.string(),
            "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\tGCCTAAGCTAA\tIIIIIIIIIII\tSA:"
            "Z:ref,29,-,6H5M,17,0;");
}

}  // namespace
}  // namespace bio
