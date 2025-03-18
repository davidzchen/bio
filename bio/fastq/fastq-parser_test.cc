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

#include "bio/fastq/fastq-parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/fastq/fastq.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(FastqParser, NextEmpty) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/empty.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(), IsOk());
  EXPECT_EQ(*sequence, nullptr);
  EXPECT_TRUE(parser->eof());
}

auto CheckSequenceEquals(const FastqSequence& expected,
                         const FastqSequence* actual) -> void {
  EXPECT_EQ(expected.name, actual->name);
  EXPECT_EQ(expected.sequence, actual->sequence);
  EXPECT_EQ(expected.quality, actual->quality);
}

TEST(FastqParser, NextMissingSequenceLine) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/invalid-missing-sequence-line.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected sequence line but got EOF")));
}

TEST(FastqParser, NextMissingQualityIdLine) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/invalid-missing-quality-id-line.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected quality ID line but got EOF")));
}

TEST(FastqParser, NextInvalidQualityIdLine) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/invalid-quality-id-line.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected quality ID: '+' or")));
}

TEST(FastqParser, NextMissingQualityLine) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/invalid-missing-quality-line.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("Expected quality line but got EOF")));
}

TEST(FastqParser, NextInvalidQualityLineLength) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/invalid-quality-line-length.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(sequence.status(),
              StatusIs(absl::StatusCode::kInvalidArgument,
                       HasSubstr("does not match quality line length")));
}

TEST(FastqParser, NextSingleSequenceWithoutQualityId) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/single-sequence-without-quality-id.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .name = "SEQ_ID",
      .sequence =
          "GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACT"
          "CACAGTTT",
      .quality = "!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextSingleSequenceWithQualityId) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/single-sequence-with-quality-id.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->Next(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36",
      .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC",
      .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextMultiSequnce) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/multiple-sequence.fastq");

  {
    absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
        parser->Next(/*truncate_name=*/false);
    EXPECT_THAT(actual_or.status(), IsOk());
    std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

    FastqSequence expected = {
        .name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
        .sequence =
            "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTTAAGGGTTTTCA"
            "AATAGA",
        .quality =
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>"
            "IIIIII/",
    };
    CheckSequenceEquals(expected, actual.get());
  }
  {
    absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
        parser->Next(/*truncate_name=*/false);
    EXPECT_THAT(actual_or.status(), IsOk());
    std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

    FastqSequence expected = {
        .name = "SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
        .sequence =
            "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATAATACGCGTCGTTT"
            "TATCAT",
        .quality =
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>"
            "IIIII-I)8I",
    };
    CheckSequenceEquals(expected, actual.get());
  }
}

TEST(FastqParser, AllEmpty) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/empty.fastq");

  absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>> sequences =
      parser->All(/*truncate_name=*/false);
  EXPECT_TRUE(sequences->empty());
}

auto CheckSequencesEquals(
    const std::vector<FastqSequence>& expected,
    const std::vector<std::unique_ptr<FastqSequence>>& actual) -> void {
  EXPECT_EQ(expected.size(), actual.size());
  for (int i = 0; i < actual.size(); ++i) {
    CheckSequenceEquals(expected[i], actual[i].get());
  }
}

TEST(FastqParser, All) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/multiple-sequence.fastq");

  absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>> actual_sequences =
      parser->All();

  std::vector<FastqSequence> expected_sequences = {
      {.name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
       .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTTAAGG"
                   "GTTTTCAAATAGA",
       .quality =
           "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>"
           "IIIIII/"},
      {.name = "SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
       .sequence = "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATAATACGC"
                   "GTCGTTTTATCAT",
       .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGI"
                  "I>IIIII-I)8I"}};
  CheckSequencesEquals(expected_sequences, *actual_sequences);
}

}  // namespace
}  // namespace bio
