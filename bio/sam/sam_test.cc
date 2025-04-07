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

#include "bio/sam/sam.h"

#include "bio/sam/cigar-parser.h"
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
      .cigar = ParseCigarOrDie("5S6M"),
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
      .cigar = ParseCigarOrDie("5S6M"),
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
      .cigar = ParseCigarOrDie("5S6M"),
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
      .cigar = ParseCigarOrDie("5S6M"),
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
