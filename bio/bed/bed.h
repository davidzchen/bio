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
#include <string>
#include <vector>

namespace bio {

struct BedSubBlock {
  uint64_t size;

  // Relative to Bed::start.
  uint64_t start;
};

struct Bed {
  std::string chromosome;
  uint64_t start;
  uint64_t end;
  bool extended;

  // Optional fields
  std::string name;

  // [0, 1000]
  uint32_t score;

  // '+'or '-'
  char strand;

  uint64_t thick_start;
  uint64_t thick_end;

  // e.x. 255,0,0
  std::string item_rgb;

  uint64_t block_count;

  std::vector<BedSubBlock> sub_blocks;
};

}  // namespace bio

#endif  // BIO_BED_BED_H_
