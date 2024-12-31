#include "bio/fastq/fastq-parser.h"

#include <memory>
#include <optional>
#include <utility>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(FastqParser, NextSequenceEmpty) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/empty.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_EQ(*sequence, nullptr);
  EXPECT_TRUE(parser->eof());
}

void CheckSequenceEquals(const FastqSequence& expected,
                         const FastqSequence* actual) {
  EXPECT_EQ(expected.name, actual->name);
  EXPECT_EQ(expected.sequence, actual->sequence);
  EXPECT_EQ(expected.quality, actual->quality);
}

TEST(FastqParser, NextSequenceSingleSequenceWithoutQualityId) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/single-sequence-without-quality-id.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .name = "@SEQ_ID",
      .sequence =
          "GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACT"
          "CACAGTTT",
      .quality = "!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextSequenceSingleSequenceWithQualityId) {
  std::unique_ptr<FastqParser> parser = FastqParser::NewOrDie(
      "bio/fastq/testdata/single-sequence-with-quality-id.fastq");

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .name = "@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36",
      .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC",
      .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextSequenceMultiSequnce) {
  std::unique_ptr<FastqParser> parser =
      FastqParser::NewOrDie("bio/fastq/testdata/multiple-sequence.fastq");

  {
    absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
        parser->NextSequence(/*truncate_name=*/false);
    EXPECT_THAT(actual_or.status(), IsOk());
    std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

    FastqSequence expected = {
        .name = "@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
        .sequence =
            "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAAC"
            "TTAAGGGTTTTCAAATAGA",
        .quality =
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>"
            "IIIIII/",
    };
    CheckSequenceEquals(expected, actual.get());
  }
  {
    absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
        parser->NextSequence(/*truncate_name=*/false);
    EXPECT_THAT(actual_or.status(), IsOk());
    std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

    FastqSequence expected = {
        .name = "@SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
        .sequence =
            "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATA"
            "ATACGCGTCGTTTTATCAT",
        .quality =
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>"
            "IIIII-I)8I",
    };
    CheckSequenceEquals(expected, actual.get());
  }
}

}  // namespace
}  // namespace bio
