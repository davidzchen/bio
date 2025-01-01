#include "bio/fasta/fasta-writer.h"

#include <memory>
#include <string>

#include "absl/status/status_matchers.h"
#include "abxl/file/file.h"
#include "abxl/file/path.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::testing::TempDir;

namespace file = ::abxl::file;

TEST(FastaWriter, Empty) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.fasta");
  std::unique_ptr<FastaWriter> writer = FastaWriter::NewOrDie(output_path);
  EXPECT_THAT(writer->Close(), IsOk());

  std::string contents;
  EXPECT_THAT(file::GetContents(output_path, &contents, file::Defaults()),
              IsOk());
  EXPECT_EQ(contents, "");
}

TEST(FastaWriter, WriteSingleSequence) {
  const std::string output_path = abxl::JoinPath(TempDir(), "out.fasta");
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
  const std::string output_path = abxl::JoinPath(TempDir(), "out.fasta");
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
  const std::string output_path = abxl::JoinPath(TempDir(), "out.fasta");
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
