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

#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "bio/fasta/fasta.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FastaParser, NextEmpty) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/empty.fasta");

  std::optional<std::unique_ptr<FastaSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_FALSE(sequence.has_value());
}

TEST(FastaParser, NextSingleSequence) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/single-sequence.fasta");

  std::optional<std::unique_ptr<FastaSequence>> actual_opt =
      parser->Next(/*truncate_name=*/false);
  EXPECT_TRUE(actual_opt.has_value());
  std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

  FastaSequence expected = {
      .name = "MCHU - Calmodulin - Human, rabbit, bovine, rat, and chicken",
      .sequence =
          "MADQLTEEQIAEFKEAFSLFDKDGDGTITTKELGTVMRSLGQNPTEAELQDMINEVDADGNGTIDFPE"
          "FLTMMARKMKDTDSEEEIREAFRVFDKDGNGYISAAELRHVMTNLGEKLTDEEVDEMIREADIDGDGQ"
          "VNYEEFVQMMTAK*",
  };
  EXPECT_EQ(actual->name, expected.name);
  EXPECT_EQ(actual->sequence, expected.sequence);
}

TEST(FastaParser, NextMultiSequence) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/multi-sequence.fasta");

  {
    std::optional<std::unique_ptr<FastaSequence>> actual_opt =
        parser->Next(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

    FastaSequence expected = {
        .name = "SEQUENCE_1",
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
        parser->Next(/*truncate_name*/ false);
    EXPECT_TRUE(actual_opt.has_value());
    std::unique_ptr<FastaSequence> actual = std::move(actual_opt.value());

    FastaSequence expected = {
        .name = "SEQUENCE_2",
        .sequence =
            "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATIGEN"
            "LVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH",
    };
    EXPECT_EQ(actual->name, expected.name);
    EXPECT_EQ(actual->sequence, expected.sequence);
  }
}

TEST(FastaParser, AllEmpty) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/empty.fasta");
  std::vector<std::unique_ptr<FastaSequence>> sequences =
      parser->All(/*truncate_name=*/false);
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

TEST(FastaParser, All) {
  std::unique_ptr<FastaParser> parser =
      FastaParser::NewOrDie("bio/fasta/testdata/multi-sequence.fasta");
  std::vector<std::unique_ptr<FastaSequence>> actual_sequences =
      parser->All(/*truncate_name=*/false);

  std::vector<FastaSequence> expected_sequences = {
      {.name = "SEQUENCE_1",
       .sequence =
           "MTEITAAMVKELRESTGAGMMDCKNALSETNGDFDKAVQLLREKGLGKAAKKADRLAAEGLVSVKV"
           "SDDFTIAAMRPSYLSYEDLDMTFVENEYKALVAELEKENEERRRLKDPNKPEHKIPQFASRKQLSD"
           "AILKEAEEKIKEELKAQGKPEKIWDNIIPGKMNSFIADNSQLDSKLTLMGQFYVMDDKKTVEQVIA"
           "EKEKEFGGKIKIVEFICFEVGEGLEKKTEDFAAEVAAQL"},
      {.name = "SEQUENCE_2",
       .sequence =
           "SATVSEINSETDFVAKNDQFIALTKDTTAHIQSNSLQSVEELHSSTINGVKFEEYLKSQIATIGEN"
           "LVVRRFATLKAGANGVVNGYIHTNGRVGVVIAAACDSAEVASKSRDLLRQICMH"},
  };
  CheckSequencesEquals(expected_sequences, actual_sequences);
}

}  // namespace
}  // namespace bio
