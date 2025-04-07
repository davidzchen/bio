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

#ifndef BIO_SAM_SAM_H_
#define BIO_SAM_SAM_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bio/sam/cigar.h"

namespace bio {

// Bitwise flags for FLAGS field in SAM entry
// N.B.:
//  1) Flag 0x02, 0x08, 0x20, 0x40, 0x80 are only meaningful when 0x01 is set
//  2) If in a read pair the information on which read is the first in the pair
//     is lost in upstream analysis, flag 0x01 should be present and 0x40 and
//     0x80 are both zero
#define SAM_READ_PAIRED 0x0001     // Read is paired in sequencing
#define SAM_PAIR_MAPPED 0x0002     // Read is mapped in proper pair
#define SAM_QUERY_UNMAPPED 0x0004  // Query sequence is unmapped
#define SAM_MATE_UNMAPPED 0x0008   // Mate is unmapped
#define SAM_QUERY_STRAND 0x0010    // Strand of query (0 forward; 1 reverse)
#define SAM_MATE_STRAND 0x0020     // Strand of mate (0 forward; 1 reverse)
#define SAM_FIRST 0x0040           // Read is first read in a pair
#define SAM_SECOND 0x0080          // Read is second read in a pair
#define SAM_NOT_PRIMARY 0x0100     // Alignment is not primary
#define SAM_FAILS_CHECKS 0x0200    // Read fails platform/vendor checks
#define SAM_DUPLICATE 0x0400       // Read is PCR or optical duplicate
#define SAM_SUPPLEMENTARY 0x800    // Supplementary alignment

// Represents a SAM entry.
struct SamEntry {
  std::string qname;  // Query name
  uint16_t flags;     // Bitwise FLAGS field
  std::string rname;  // Reference sequence name
  uint32_t pos;       // 1-based leftmost position/coordinate of clipped seq.
  uint8_t mapq;       // Mapping quality
  Cigar cigar;        // Extended CIGAR string
  std::string rnext;  // Mate reference sequence name ("=" if same as rname)
  uint32_t pnext;     // 1-based leftmost mate position of clipped sequence
  int32_t tlen;       // Observed Template Length
  std::optional<std::string> seq;   // Query sequence
  std::optional<std::string> qual;  // Query quality string
  std::vector<std::string> tags;    // Optional tags

  // Serializes the SamEntry to its string representation.
  auto string() const -> std::string;
};

}  // namespace bio

#endif  // BIO_SAM_SAM_H_
