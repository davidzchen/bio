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

#include "bio/fastq/fastq-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"
#include "gxl/file/path.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::gxl::file;

TEST(FastqWriter, Empty) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastqWriter> writer = FastqWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(FastqWriter, WriteSingleSequence) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fastq");
  std::unique_ptr<FastqWriter> writer = FastqWriter::NewOrDie(output_path);

  EXPECT_THAT(
      writer->Write({
          .name = "SEQ_ID",
          .sequence =
              "GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACTCACAGTTT",
          .quality =
              "!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65",
      }),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            R"(@SEQ_ID
GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACTCACAGTTT
+
!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65
)");
}

TEST(FastqWriter, WriteMultipleSequences) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fastq");
  std::unique_ptr<FastqWriter> writer = FastqWriter::NewOrDie(output_path);

  EXPECT_THAT(
      writer->Write(
          {.name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
           .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTT"
                       "AAGGGTTTTCAAATAGA",
           .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIII"
                      "DIIIIIII>IIIIII/"}),
      IsOk());
  EXPECT_THAT(
      writer->Write(
          {.name = "SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
           .sequence = "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATA"
                       "ATACGCGTCGTTTTATCAT",
           .quality =
               "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>"
               "IIIII-I)8I"}),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            R"(@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72
GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTTAAGGGTTTTCAAATAGA
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>IIIIII/
@SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72
GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATAATACGCGTCGTTTTATCAT
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>IIIII-I)8I
)");
}

TEST(FastqWriter, WriteVector) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fastq");
  std::unique_ptr<FastqWriter> writer = FastqWriter::NewOrDie(output_path);

  EXPECT_THAT(
      writer->Write(
          {{.name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
            .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACT"
                        "TAAGGGTTTTCAAATAGA",
            .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIII"
                       "IDIIIIIII>IIIIII/"},
           {.name = "SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
            .sequence = "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATA"
                        "ATACGCGTCGTTTTATCAT",
            .quality =
                "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>"
                "IIIII-I)8I"}}),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            R"(@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72
GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTTAAGGGTTTTCAAATAGA
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>IIIIII/
@SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72
GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATAATACGCGTCGTTTTATCAT
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>IIIII-I)8I
)");
}

}  // namespace
}  // namespace bio
