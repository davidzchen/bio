#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <utility>

#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/common/sequence.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;

TEST(FastaParser, NextSequenceEmpty) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/empty.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  std::optional<std::unique_ptr<Sequence>> sequence =
      parser->NextSequence(/*truncate_name*/ false);
  EXPECT_FALSE(sequence.has_value());
}

TEST(FastaParser, NextSequenceSingleSequence) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/single-sequence.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  std::optional<std::unique_ptr<Sequence>> actual_opt =
      parser->NextSequence(/*truncate_name*/ false);
  EXPECT_TRUE(actual_opt.has_value());
  std::unique_ptr<Sequence> actual = std::move(actual_opt.value());

  Sequence expected = {
      .name = ">MCHU - Calmodulin - Human, rabbit, bovine, rat, and chicken",
      .sequence =
          "MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADGNGTIDFPE"
          "FLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDEEVDEMIREADIDGDGQ"
          "VNYEEFVQMMTAK*",
      .size = 150,
  };
  EXPECT_EQ(actual->name, expected.name);
  EXPECT_EQ(actual->sequence, expected.sequence);
  EXPECT_EQ(actual->size, expected.size);
}

TEST(FastaParser, NextSequenceMultiSequence) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/multi-sequence.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  {
    std::optional<std::unique_ptr<Sequence>> actual_opt =
        parser->NextSequence(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<Sequence> actual = std::move(actual_opt.value());

    Sequence expected = {
        .name = ">SEQUENCE_1",
        .sequence =
            "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLVSVKV"
            "SDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQFASRKQLSD"
            "AILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYVMDDKKTVEQVIA"
            "EKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL",
        .size = 237,
    };
    EXPECT_EQ(actual->name, expected.name);
    EXPECT_EQ(actual->sequence, expected.sequence);
    EXPECT_EQ(actual->size, expected.size);
  }
  {
    std::optional<std::unique_ptr<Sequence>> actual_opt =
        parser->NextSequence(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<Sequence> actual = std::move(actual_opt.value());

    Sequence expected = {
        .name = ">SEQUENCE_2",
        .sequence =
            "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATIGEN"
            "LVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH",
        .size = 120,
    };
    EXPECT_EQ(actual->name, expected.name);
    EXPECT_EQ(actual->sequence, expected.sequence);
    EXPECT_EQ(actual->size, expected.size);
  }
}

}  // namespace
}  // namespace bio
