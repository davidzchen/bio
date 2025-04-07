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

#include "bio/sam/sam-parser.h"

#include <memory>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/sam/cigar-parser.h"
#include "bio/sam/sam.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(SamParser, NextEmpty) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/empty.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), IsOk());
  EXPECT_EQ(*entry, nullptr);
  EXPECT_TRUE(parser->eof());
}

TEST(SamParser, NextInvalidNumFields) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-num-fields.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid number of fields")));
}

TEST(SamParser, NextInvalidFlags) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-flags.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid flags format")));
}

TEST(SamParser, NextInvalidPos) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-pos.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid pos format")));
}

TEST(SamParser, NextInvalidMapq) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-mapq.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Failed to parse 'mapq'")));
}

TEST(SamParser, NextInvalidPnext) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-pnext.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid pnext format")));
}

TEST(SamParser, NextInvalidTlen) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/invalid-tlen.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> entry = parser->Next();
  EXPECT_THAT(entry.status(), StatusIs(absl::StatusCode::kInvalidArgument,
                                       HasSubstr("Invalid tlen format")));
}

auto CheckSamEntryEquals(const SamEntry* actual, const SamEntry& expected)
    -> void {
  EXPECT_EQ(actual->qname, expected.qname);
  EXPECT_EQ(actual->flags, expected.flags);
  EXPECT_EQ(actual->rname, expected.rname);
  EXPECT_EQ(actual->pos, expected.pos);
  EXPECT_EQ(actual->mapq, expected.mapq);
  EXPECT_EQ(actual->cigar, expected.cigar);
  EXPECT_EQ(actual->rnext, expected.rnext);
  EXPECT_EQ(actual->pnext, expected.pnext);
  EXPECT_EQ(actual->tlen, expected.tlen);
  EXPECT_EQ(actual->seq, expected.seq);
  EXPECT_EQ(actual->qual, expected.qual);
  EXPECT_EQ(actual->tags.size(), expected.tags.size());
  for (int i = 0; i < actual->tags.size(); ++i) {
    EXPECT_EQ(actual->tags[i], expected.tags[i]);
  }
}

TEST(SamParser, NextMinimalFields) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/minimal-fields.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> actual = parser->Next();
  EXPECT_THAT(actual.status(), IsOk());
  EXPECT_NE(*actual, nullptr);
  SamEntry expected = {
      .qname = "r001",
      .flags = SAM_READ_PAIRED | SAM_PAIR_MAPPED | SAM_MATE_STRAND | SAM_FIRST,
      .rname = "ref",
      .pos = 7,
      .mapq = 30,
      .cigar = ParseCigarOrDie("8M2I4M1D3M"),
      .rnext = "=",
      .pnext = 37,
      .tlen = 39,
      .seq = "TTAGATAAAGGATACTG",
  };
  CheckSamEntryEquals(actual->get(), expected);
}

TEST(SamParser, NextWithTags) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/with-tags.sam");

  absl::StatusOr<std::unique_ptr<SamEntry>> actual = parser->Next();
  EXPECT_THAT(actual.status(), IsOk());
  EXPECT_NE(*actual, nullptr);
  SamEntry expected = {
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
  CheckSamEntryEquals(actual->get(), expected);
}

TEST(SamParser, NextMultipleEntries) {
  std::unique_ptr<SamParser> parser =
      SamParser::NewOrDie("bio/sam/testdata/multiple-entries.sam");

  {
    absl::StatusOr<std::unique_ptr<SamEntry>> actual = parser->Next();
    EXPECT_THAT(actual.status(), IsOk());
    EXPECT_NE(*actual, nullptr);
    SamEntry expected = {
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
    CheckSamEntryEquals(actual->get(), expected);
  }
  {
    absl::StatusOr<std::unique_ptr<SamEntry>> actual = parser->Next();
    EXPECT_THAT(actual.status(), IsOk());
    EXPECT_NE(*actual, nullptr);
    SamEntry expected = {
        .qname = "r003",
        .flags = SAM_QUERY_STRAND | SAM_SUPPLEMENTARY,
        .rname = "ref",
        .pos = 29,
        .mapq = 17,
        .cigar = ParseCigarOrDie("6H5M"),
        .rnext = "*",
        .pnext = 0,
        .tlen = 0,
        .seq = "TAGGC",
        .tags = {"SA:Z:ref,9,+,5S6M,30,1;"},
    };
    CheckSamEntryEquals(actual->get(), expected);
  }
}

}  // namespace
}  // namespace bio
