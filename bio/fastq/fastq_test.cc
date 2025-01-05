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

#include "bio/fastq/fastq.h"

#include <string>

#include "gtest/gtest.h"

namespace bio {
namespace {

TEST(FastqSequence, StringEmptySequence) {
  FastqSequence sequence = {
      .name = "SEQUENCE",
      .sequence = "",
      .quality = "",
  };
  EXPECT_EQ(sequence.string(), "@SEQUENCE\n\n+\n");
}

TEST(FastqSequence, String) {
  FastqSequence sequence = {
      .name = "SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36",
      .sequence = "GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC",
      .quality = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC",
  };
  EXPECT_EQ(sequence.string(),
            R"(@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=36
GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACC
+
IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9IC)");
}

}  // namespace
}  // namespace bio
