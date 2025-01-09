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

#include "bio/fasta/fasta-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "gtest/gtest.h"
#include "gxl/file/file.h"
#include "gxl/file/path.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::gxl::file;

TEST(FastaWriter, Empty) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastaWriter> writer = FastaWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(FastaWriter, WriteSingleSequence) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastaWriter> writer = FastaWriter::NewOrDie(output_path);
  EXPECT_THAT(
      writer->Write({
          .name = "MCHU - Calmodulin - Human, rabbit, bovine, rat, and chicken",
          .sequence = "MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEV"
                      "DADGNGTIDFPE"
                      "FLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDEEVDE"
                      "MIREADIDGDGQ"
                      "VNYEEFVQMMTAK*",
      }),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents,
            R"(>MCHU - Calmodulin - Human, rabbit, bovine, rat, and chicken
MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADG
NGTIDFPEFLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDE
EVDEMIREADIDGDGQVNYEEFVQMMTAK*
)");
}

TEST(FastaWriter, WriteMultipleSequences) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastaWriter> writer = FastaWriter::NewOrDie(output_path);
  EXPECT_THAT(
      writer->Write(
          {{.name = "SEQUENCE_1",
            .sequence =
                "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLV"
                "SVKVSDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQF"
                "ASRKQLSDAILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYV"
                "MDDKKTVEQVIAEKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL"}}),
      IsOk());
  EXPECT_THAT(
      writer->Write({
          {.name = "SEQUENCE_2",
           .sequence =
               "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATI"
               "GENLVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH"},
      }),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_THAT(contents,
              R"(>SEQUENCE_1
MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEG
LVSVKVSDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHK
IPQFASRKQLSDAILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTL
MGQFYVMDDKKTVEQVIAEKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL
>SEQUENCE_2
SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQI
ATIGENLVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH
)");
}

TEST(FastaWriter, WriteMultipleSequencesArray) {
  const std::string output_path = gxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastaWriter> writer = FastaWriter::NewOrDie(output_path);
  EXPECT_THAT(
      writer->Write({
          {.name = "SEQUENCE_1",
           .sequence =
               "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLV"
               "SVKVSDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQF"
               "ASRKQLSDAILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYV"
               "MDDKKTVEQVIAEKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL"},
          {.name = "SEQUENCE_2",
           .sequence =
               "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATI"
               "GENLVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH"},
      }),
      IsOk());
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_THAT(contents,
              R"(>SEQUENCE_1
MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEG
LVSVKVSDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHK
IPQFASRKQLSDAILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTL
MGQFYVMDDKKTVEQVIAEKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL
>SEQUENCE_2
SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQI
ATIGENLVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH
)");
}

}  // namespace
}  // namespace bio
