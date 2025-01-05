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

#include "bio/bed/bed-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "abxl/file/file.h"
#include "abxl/file/path.h"
#include "bio/bed/bed.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::abxl::file;

TEST(BedWriter, Empty) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedWriter> writer = BedWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(BedWriter, WriteSingleEntry) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedWriter> writer = BedWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 0,
                  .end = 1000,
                  .name = "Test",
                  .score = 900,
                  .strand = BedStrand::kSense,
                  .thick_start = 50,
                  .thick_end = 1000,
                  .item_rgb = "255,0,0",
                  .sub_blocks = {{.size = 567, .start = 0},
                                 {.size = 488, .start = 588}},
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(
      contents,
      "chr7\t0\t1000\tTest\t900\t+\t50\t1000\t255,0,0\t2\t567,488\t0,588\n");
}

TEST(BedWriter, WriteMultipleEntries) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedWriter> writer = BedWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 0,
                  .end = 1000,
                  .name = "Test",
                  .score = 900,
                  .strand = BedStrand::kSense,
                  .thick_start = 50,
                  .thick_end = 1000,
                  .item_rgb = "255,0,0",
                  .sub_blocks = {{.size = 567, .start = 0},
                                 {.size = 488, .start = 588}},
              }),
              IsOk());
  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 200,
                  .end = 1200,
                  .name = "Test",
                  .score = 800,
                  .strand = BedStrand::kAntisense,
                  .thick_start = 50,
                  .thick_end = 1000,
                  .item_rgb = "0,0,255",
                  .sub_blocks = {{.size = 567, .start = 0},
                                 {.size = 488, .start = 588}},
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(
      contents,
      "chr7\t0\t1000\tTest\t900\t+\t50\t1000\t255,0,0\t2\t567,488\t0,588\n"
      "chr7\t200\t1200\tTest\t800\t-\t50\t1000\t0,0,255\t2\t567,488\t0,588\n");
}

TEST(BedWriter, WriteVector) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedWriter> writer = BedWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  {.chromosome = "chr7",
                   .start = 0,
                   .end = 1000,
                   .name = "Test",
                   .score = 900,
                   .strand = BedStrand::kSense,
                   .thick_start = 50,
                   .thick_end = 1000,
                   .item_rgb = "255,0,0",
                   .sub_blocks = {{.size = 567, .start = 0},
                                  {.size = 488, .start = 588}}},
                  {
                      .chromosome = "chr7",
                      .start = 200,
                      .end = 1200,
                      .name = "Test",
                      .score = 800,
                      .strand = BedStrand::kAntisense,
                      .thick_start = 50,
                      .thick_end = 1000,
                      .item_rgb = "0,0,255",
                      .sub_blocks = {{.size = 567, .start = 0},
                                     {.size = 488, .start = 588}},
                  },
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(
      contents,
      "chr7\t0\t1000\tTest\t900\t+\t50\t1000\t255,0,0\t2\t567,488\t0,588\n"
      "chr7\t200\t1200\tTest\t800\t-\t50\t1000\t0,0,255\t2\t567,488\t0,588\n");
}

}  // namespace
}  // namespace bio
