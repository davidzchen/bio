#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FastaParser, NextSequenceEmpty) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/empty.fasta");

  std::optional<std::unique_ptr<FastaSequence>> sequence =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_FALSE(sequence.has_value());
}

TEST(FastaParser, NextSequenceSingleSequence) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/single-sequence.fasta");

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
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/multi-sequence.fasta");

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

TEST(FastaParser, ReadAllSequencesEmpty) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/empty.fasta");
  std::vector<std::unique_ptr<FastaSequence>> sequences =
      parser->ReadAllSequences(/*truncate_name=*/false);
  EXPECT_TRUE(sequences.empty());
}

auto CheckSequencesEquals(
    const std::vector<FastaSequence>& expected,
    const std::vector<std::unique_ptr<FastaSequence>>& actual) {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < actual.size(); ++i) {
    EXPECT_EQ(actual[i]->name, expected[i].name);
    EXPECT_EQ(actual[i]->sequence, expected[i].sequence);
  }
}

TEST(FastaParser, ReadAllSequences) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/multi-sequence.fasta");
  std::vector<std::unique_ptr<FastaSequence>> actual_sequences =
      parser->ReadAllSequences(/*truncate_name=*/false);

  std::vector<FastaSequence> expected_sequences = {
      {.name = ">SEQUENCE_1",
       .sequence =
           "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLVSVKV"
           "SDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQFASRKQLSD"
           "AILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYVMDDKKTVEQVIA"
           "EKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL"},
      {.name = ">SEQUENCE_2",
       .sequence =
           "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATIGEN"
           "LVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH"},
  };
  CheckSequencesEquals(expected_sequences, actual_sequences);
}

}  // namespace
}  // namespace bio
