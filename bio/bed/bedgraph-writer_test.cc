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

#include "bio/bed/bedgraph-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "abxl/file/file.h"
#include "abxl/file/path.h"
#include "bio/bed/bedgraph.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::abxl::file;

TEST(BedGraphWriter, Empty) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bedgraph");
  std::unique_ptr<BedGraphWriter> writer =
      BedGraphWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(BedGraphWriter, WriteSingleEntry) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bedgraph");
  std::unique_ptr<BedGraphWriter> writer =
      BedGraphWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 0,
                  .end = 2000,
                  .value = -1.00,
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "chr7\t0\t2000\t-1.00\n");
}

TEST(BedGraphWriter, WriteMultipleEntries) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedGraphWriter> writer =
      BedGraphWriter::NewOrDie(output_path);

  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 0,
                  .end = 1000,
                  .value = -1.00,
              }),
              IsOk());
  EXPECT_THAT(writer->Write({
                  .chromosome = "chr7",
                  .start = 200,
                  .end = 1200,
                  .value = -2.0,
              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "chr7\t0\t1000\t-1.00\n"
            "chr7\t200\t1200\t-2.00\n");
}

TEST(BedGraphWriter, WriteVector) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.bed");
  std::unique_ptr<BedGraphWriter> writer =
      BedGraphWriter::NewOrDie(output_path);

  EXPECT_THAT(
      writer->Write(
          {{.chromosome = "chr7", .start = 0, .end = 1000, .value = -1.00},
           {.chromosome = "chr7", .start = 200, .end = 1200, .value = -2.0}}),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "chr7\t0\t1000\t-1.00\n"
            "chr7\t200\t1200\t-2.00\n");
}

}  // namespace
}  // namespace bio
