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

#include "bio/mrf/mrf-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "bio/common/sequence.h"
#include "bio/mrf/mrf.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"
#include "gxl/file/path.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;
using ::testing::TempDir;

namespace file = ::gxl::file;

TEST(MrfWriter, Empty) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);

  EXPECT_THAT(writer->Start(), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "AlignmentBlocks\n");
}

TEST(MrfWriter, EmptyAllColumns) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);

  EXPECT_THAT(writer->Start(), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "AlignmentBlocks\tSequence\tQualityScores\tQueryId\n");
}

TEST(MrfWriter, WriteCalledBeforeStart) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);

  MrfEntry dummy;
  EXPECT_THAT(writer->Write(dummy),
              StatusIs(absl::StatusCode::kFailedPrecondition,
                       HasSubstr("Start() must be called before Write()")));
}

TEST(MrfWriter, CloseCalledBeforeStart) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);

  EXPECT_THAT(writer->Close(),
              StatusIs(absl::StatusCode::kFailedPrecondition,
                       HasSubstr("Start() must be called before Close()")));
}

TEST(MrfWriter, WriteSingleEndEntryRequiredColumn) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  MrfEntry entry = {
      .is_paired_end = false,
      .read1 = {.blocks = {{.target_name = "chr4",
                            .strand = Strand::kAntisense,
                            .target_start = 1221,
                            .target_end = 1270,
                            .query_start = 1,
                            .query_end = 50}}},
  };
  EXPECT_THAT(writer->Write(entry), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "AlignmentBlocks\n"
            "chr4:-:1221:1270:1:50\n");
}

TEST(MrfWriter, WriteSingleEndEntryAllColumns) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  MrfEntry entry = {
      .is_paired_end = false,
      .read1 =
          {
              .blocks = {{.target_name = "chr4",
                          .strand = Strand::kAntisense,
                          .target_start = 1221,
                          .target_end = 1270,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "ACTCGAA",
              .quality_scores = "IIIIIII",
              .query_id = "1",
          },
  };
  EXPECT_THAT(writer->Write(entry), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "AlignmentBlocks\tSequence\tQualityScores\tQueryId\n"
            "chr4:-:1221:1270:1:50\tACTCGAA\tIIIIIII\t1\n");
}

TEST(MrfWriter, WritePairedEndEntry) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  MrfEntry entry = {
      .is_paired_end = true,
      .read1 = {.blocks = {{.target_name = "chr2",
                            .strand = Strand::kSense,
                            .target_start = 601,
                            .target_end = 630,
                            .query_start = 1,
                            .query_end = 30},
                           {.target_name = "chr2",
                            .strand = Strand::kSense,
                            .target_start = 921,
                            .target_end = 940,
                            .query_start = 31,
                            .query_end = 50}}},
      .read2 = {.blocks = {{
                    .target_name = "chr2",
                    .strand = Strand::kSense,
                    .target_start = 1401,
                    .target_end = 1450,
                    .query_start = 1,
                    .query_end = 50,
                }}},
  };
  EXPECT_THAT(writer->Write(entry), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "AlignmentBlocks\n"
            "chr2:+:601:630:1:30,chr2:+:921:940:31:50|chr2:+:1401:1450:1:50\n");
}

TEST(MrfWriter, WritePairedEndAllColumns) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks, MrfColumn::kSequence,
                    MrfColumn::kQualityScores, MrfColumn::kQueryId});
  header.AddComments({"Paired End All Columns"});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  MrfEntry entry = {
      .is_paired_end = true,
      .read1 =
          {
              .blocks = {{.target_name = "chr2",
                          .strand = Strand::kSense,
                          .target_start = 601,
                          .target_end = 630,
                          .query_start = 1,
                          .query_end = 30}},
              .sequence = "ACTG",
              .quality_scores = "IIII",
              .query_id = "1",
          },
      .read2 =
          {
              .blocks = {{.target_name = "chr2",
                          .strand = Strand::kSense,
                          .target_start = 1401,
                          .target_end = 1450,
                          .query_start = 1,
                          .query_end = 50}},
              .sequence = "GCTA",
              .quality_scores = "JJJJ",
              .query_id = "2",
          },
  };
  EXPECT_THAT(writer->Write(entry), IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "# Paired End All Columns\n"
            "AlignmentBlocks\tSequence\tQualityScores\tQueryId\n"
            "chr2:+:601:630:1:30|chr2:+:1401:1450:1:50\t"
            "ACTG|GCTA\tIIII|JJJJ\t1|2\n");
}

TEST(MrfWriter, WriteMultipleEntries) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  EXPECT_THAT(writer->Write({
                  .is_paired_end = false,
                  .read1 = {.blocks = {{.target_name = "chr4",
                                        .strand = Strand::kAntisense,
                                        .target_start = 1221,
                                        .target_end = 1270,
                                        .query_start = 1,
                                        .query_end = 50}}},

              }),
              IsOk());
  EXPECT_THAT(writer->Write({
                  .is_paired_end = false,
                  .read1 = {.blocks = {{.target_name = "chr4",
                                        .strand = Strand::kAntisense,
                                        .target_start = 2221,
                                        .target_end = 2270,
                                        .query_start = 1,
                                        .query_end = 50}}},

              }),
              IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "AlignmentBlocks\n"
            "chr4:-:1221:1270:1:50\n"
            "chr4:-:2221:2270:1:50\n");
}

TEST(MrfWriter, WriteMultipleEntriesVector) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.mrf");
  MrfHeader header({MrfColumn::kAlignmentBlocks});
  std::unique_ptr<MrfWriter> writer = MrfWriter::NewOrDie(output_path, &header);
  EXPECT_THAT(writer->Start(), IsOk());

  EXPECT_THAT(
      writer->Write({{.is_paired_end = false,
                      .read1 = {.blocks = {{.target_name = "chr4",
                                            .strand = Strand::kAntisense,
                                            .target_start = 1221,
                                            .target_end = 1270,
                                            .query_start = 1,
                                            .query_end = 50}}}

                     },
                     {.is_paired_end = false,
                      .read1 = {.blocks = {{.target_name = "chr4",
                                            .strand = Strand::kAntisense,
                                            .target_start = 2221,
                                            .target_end = 2270,
                                            .query_start = 1,
                                            .query_end = 50}}}

                     }}),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            "AlignmentBlocks\n"
            "chr4:-:1221:1270:1:50\n"
            "chr4:-:2221:2270:1:50\n");
}

}  // namespace
}  // namespace bio
