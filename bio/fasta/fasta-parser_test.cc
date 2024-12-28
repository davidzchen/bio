#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <utility>

#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;

TEST(FastaParser, NextSequenceEmpty) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/empty.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  std::optional<std::unique_ptr<FastaSequence>> sequence =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_FALSE(sequence.has_value());
}

TEST(FastaParser, NextSequenceSingleSequence) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/single-sequence.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  std::optional<std::unique_ptr<FastaSequence>> actual_opt =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_TRUE(actual_opt.has_value());
  std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

  FastaSequence expected = {
      .name = ">MCHU - Calmodulin - Human, rabbit, bovine, rat, and chicken",
      .sequence =
          "MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADGNGTIDFPE"
          "FLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDEEVDEMIREADIDGDGQ"
          "VNYEEFVQMMTAK*",
  };
  EXPECT_EQ(actual->name, expected.name);
  EXPECT_EQ(actual->sequence, expected.sequence);
}

TEST(FastaParser, NextSequenceMultiSequence) {
  absl::StatusOr<std::unique_ptr<FastaParser>> parser_or =
      FastaParser::New("bio/fasta/testdata/multi-sequence.fasta");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastaParser> parser = std::move(parser_or.value());

  {
    std::optional<std::unique_ptr<FastaSequence>> actual_opt =
        parser->NextSequence(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

    FastaSequence expected = {
        .name = ">SEQUENCE_1",
        .sequence =
            "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLVSVKV"
            "SDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQFASRKQLSD"
            "AILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYVMDDKKTVEQVIA"
            "EKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL",
    };
    EXPECT_EQ(actual->name, expected.name);
    EXPECT_EQ(actual->sequence, expected.sequence);
  }
  {
    std::optional<std::unique_ptr<FastaSequence>> actual_opt =
        parser->NextSequence(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

    FastaSequence expected = {
        .name = ">SEQUENCE_2",
        .sequence =
            "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATIGEN"
            "LVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH",
    };
    EXPECT_EQ(actual->name, expected.name);
    EXPECT_EQ(actual->sequence, expected.sequence);
  }
}

}  // namespace
}  // namespace bio
