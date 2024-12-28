#include "bio/fastq/fastq-parser.h"

#include <memory>
#include <optional>
#include <utility>

#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "bio/common/sequence.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::HasSubstr;

TEST(FastqParser, NextSequenceEmpty) {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser_or =
      FastqParser::New("bio/fastq/testdata/empty.fastq");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastqParser> parser = std::move(parser_or.value());

  absl::StatusOr<std::unique_ptr<FastqSequence>> sequence =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_EQ(*sequence, nullptr);
  EXPECT_TRUE(parser->eof());
}

void CheckSequenceEquals(const FastqSequence& expected,
                         const FastqSequence* actual) {
  EXPECT_EQ(expected.sequence.name, actual->sequence.name);
  EXPECT_EQ(expected.sequence.sequence, actual->sequence.sequence);
  EXPECT_EQ(expected.sequence.size, actual->sequence.size);
  EXPECT_EQ(expected.quality, actual->quality);
}

TEST(FastqParser, NextSequenceSingleSequenceWithoutQualityId) {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser_or = FastqParser::New(
      "bio/fastq/testdata/single-sequence-without-quality-id.fastq");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastqParser> parser = std::move(parser_or.value());

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .sequence = {.name = "@SEQ_ID",
                   .sequence =
                       "GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACT"
                       "CACAGTTT",
                   .size = 60},
      .quality = "!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextSequenceSingleSequenceWithQualityId) {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser_or = FastqParser::New(
      "bio/fastq/testdata/single-sequence-with-quality-id.fastq");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastqParser> parser = std::move(parser_or.value());

  absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
      parser->NextSequence(/*truncate_name=*/false);
  EXPECT_THAT(actual_or.status(), IsOk());
  std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

  FastqSequence expected = {
      .sequence =
          {.name = "@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36",
           .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC",
           .size = 36},
      .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC",
  };
  CheckSequenceEquals(expected, actual.get());
}

TEST(FastqParser, NextSequenceMultiSequnce) {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser_or =
      FastqParser::New("bio/fastq/testdata/multiple-sequence.fastq");
  EXPECT_THAT(parser_or.status(), IsOk());
  std::unique_ptr<FastqParser> parser = std::move(parser_or.value());

  {
    absl::StatusOr<std::unique_ptr<FastqSequence>> actual_or =
        parser->NextSequence(/*truncate_name=*/false);
    EXPECT_THAT(actual_or.status(), IsOk());
    std::unique_ptr<FastqSequence> actual = std::move(actual_or.value());

    FastqSequence expected = {
        .sequence =
            {.name = "@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72",
             .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAAC"
                         "TTAAGGGTTTTCAAATAGA",
             .size = 72},
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
        .sequence =
            {.name = "@SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72",
             .sequence = "GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATA"
                         "ATACGCGTCGTTTTATCAT",
             .size = 72},
        .quality =
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>"
            "IIIII-I)8I",
    };
    CheckSequenceEquals(expected, actual.get());
  }
}

}  // namespace
}  // namespace bio
