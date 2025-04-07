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

#include "bio/sam/sam-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "bio/sam/cigar-parser.h"
#include "bio/sam/sam.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"
#include "gxl/file/path.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::gxl::file;

TEST(SamWriter, Empty) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.sam");
  std::unique_ptr<SamWriter> writer = SamWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(SamWriter, WriteSingleEntry) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.sam");
  std::unique_ptr<SamWriter> writer = SamWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  .qname = "r001",
                  .flags = SAM_READ_PAIRED | SAM_PAIR_MAPPED | SAM_MATE_STRAND |
                           SAM_FIRST,
                  .rname = "ref",
                  .pos = 7,
                  .mapq = 30,
                  .cigar = ParseCigarOrDie("8M2I4M1D3M"),
                  .rnext = "=",
                  .pnext = 37,
                  .tlen = 39,
                  .seq = "TTAGATAAAGGATACTG",
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(
      contents,
      "r001\t99\tref\t7\t30\t8M2I4M1D3M\t=\t37\t39\tTTAGATAAAGGATACTG\t*\n");
}

TEST(SamWriter, WriteMultipleEntries) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.sam");
  std::unique_ptr<SamWriter> writer = SamWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
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
              }),
              IsOk());
  EXPECT_THAT(writer->Write({
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
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\tGCCTAAGCTAA\t*\tSA:Z:ref,29,-,"
            "6H5M,17,0;\n"
            "r003\t2064\tref\t29\t17\t6H5M\t*\t0\t0\tTAGGC\t*\tSA:Z:ref,9,+,"
            "5S6M,30,1;\n");
}

TEST(SamWriter, WriteVector) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.sam");
  std::unique_ptr<SamWriter> writer = SamWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({{.qname = "r003",
                              .flags = 0,
                              .rname = "ref",
                              .pos = 9,
                              .mapq = 30,
                              .cigar = ParseCigarOrDie("5S6M"),
                              .rnext = "*",
                              .pnext = 0,
                              .tlen = 0,
                              .seq = "GCCTAAGCTAA",
                              .tags = {"SA:Z:ref,29,-,6H5M,17,0;"}},
                             {.qname = "r003",
                              .flags = SAM_QUERY_STRAND | SAM_SUPPLEMENTARY,
                              .rname = "ref",
                              .pos = 29,
                              .mapq = 17,
                              .cigar = ParseCigarOrDie("6H5M"),
                              .rnext = "*",
                              .pnext = 0,
                              .tlen = 0,
                              .seq = "TAGGC",
                              .tags = {"SA:Z:ref,9,+,5S6M,30,1;"}}}),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "r003\t0\tref\t9\t30\t5S6M\t*\t0\t0\tGCCTAAGCTAA\t*\tSA:Z:ref,29,-,"
            "6H5M,17,0;\n"
            "r003\t2064\tref\t29\t17\t6H5M\t*\t0\t0\tTAGGC\t*\tSA:Z:ref,9,+,"
            "5S6M,30,1;\n");
}

}  // namespace
}  // namespace bio
