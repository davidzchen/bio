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

#ifndef BIO_BED_BED_H_
#define BIO_BED_BED_H_

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <vector>

#include "bio/common/sequence.h"

namespace bio {

// Represents a subfeature within a feature.
struct BedSubBlock {
  // Block size.
  uint64_t size;

  // Block start position, relative to Bed::start.
  uint64_t start;
};

// Represents a BED entry.
struct BedEntry {
  // Chromosome name. The name must be between 1 and 255 charcters long,
  // inclusive.
  std::string chromosome;

  // Feature start position.
  uint64_t start;

  // Feature end position.
  uint64_t end;

  // Feature description. The description must be between 1 and 255 characters
  // long, inclusive.
  std::optional<std::string> name;

  // A numerical value between 0 and 1000 inclusive.
  std::optional<uint32_t> score;

  // Feature strand. This is parsed from '+' (sense or coding ) or '-'
  // (antisense or complement) from the file.
  std::optional<Strand> strand;

  // Thick start position.
  std::optional<uint64_t> thick_start;

  // Thick end position.
  std::optional<uint64_t> thick_end;

  // Display color. This consists of a triple of integers between 0 and 255
  // inclusive separated by commas (e.x. 255,0,0). Note taht an `item_rgb` of 0
  // is a special case and is visualized differently from a value of 0,0,0.
  std::optional<std::string> item_rgb;

  // Blocks, linear subfeatures within a feature. The block count is given by
  // `sub_blocks.size()`.
  std::vector<BedSubBlock> sub_blocks;

  // Returns the number of sub blocks in this entry.
  auto block_count() const -> size_t { return sub_blocks.size(); }

  // Serializes this BedEntry to its BED string format.
  auto string() const -> std::string;
};

}  // namespace bio

#endif  // BIO_BED_BED_H_
