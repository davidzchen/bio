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

#include "bio/sam/sam.h"

#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"

namespace bio {

auto SamEntry::string() const -> std::string {
  std::string str =
      absl::StrFormat("%s\t%d\t%s\t%d\t%d\t%s\t%s\t%d\t%d", qname, flags, rname,
                      pos, mapq, cigar.string(), rnext, pnext, tlen);

  if (seq.has_value()) {
    absl::StrAppend(&str, absl::StrFormat("\t%s", *seq));
  } else {
    absl::StrAppend(&str, "\t*");
  }

  if (qual.has_value()) {
    absl::StrAppend(&str, absl::StrFormat("\t%s", *qual));
  } else {
    absl::StrAppend(&str, "\t*");
  }

  if (!tags.empty()) {
    absl::StrAppend(&str, "\t", absl::StrJoin(tags, ","));
  }

  return str;
}

}  // namespace bio
