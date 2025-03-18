#include "bio/sam/sam.h"

#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"

namespace bio {

auto SamEntry::string() const -> std::string {
  std::string str =
      absl::StrFormat("%s\t%d\t%s\t%d\t%d\t%s\t%s\t%d\t%d", qname, flags, rname,
                      pos, mapq, cigar, rnext, pnext, tlen);

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
