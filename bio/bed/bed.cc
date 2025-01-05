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

#include "bio/bed/bed.h"

#include <cstdint>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "bio/common/sequence.h"

namespace bio {
namespace {

static constexpr char kStrandSense[] = "+";
static constexpr char kStrandAntisense[] = "-";

}  // namespace

auto BedEntry::string() const -> std::string {
  std::vector<std::string> parts;
  parts.push_back(chromosome);
  parts.push_back(absl::StrCat(start));
  parts.push_back(absl::StrCat(end));
  if (name.has_value()) {
    parts.push_back(*name);
  }
  if (score.has_value()) {
    parts.push_back(absl::StrCat(*score));
  }
  if (strand.has_value()) {
    parts.push_back(*strand == Strand::kSense ? kStrandSense
                                              : kStrandAntisense);
  }
  if (thick_start.has_value()) {
    parts.push_back(absl::StrCat(*thick_start));
  }
  if (thick_end.has_value()) {
    parts.push_back(absl::StrCat(*thick_end));
  }
  if (item_rgb.has_value()) {
    parts.push_back(*item_rgb);
  }
  if (!sub_blocks.empty()) {
    parts.push_back(absl::StrCat(block_count()));
    std::vector<uint64_t> block_sizes;
    std::vector<uint64_t> block_starts;
    for (const auto& sub_block : sub_blocks) {
      block_sizes.push_back(sub_block.size);
      block_starts.push_back(sub_block.start);
    }
    parts.push_back(absl::StrJoin(block_sizes, ","));
    parts.push_back(absl::StrJoin(block_starts, ","));
  }

  return absl::StrJoin(parts, "\t");
}

}  // namespace bio
