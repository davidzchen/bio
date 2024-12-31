#include "bio/common/strings.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace bio {

auto FirstWord(absl::string_view str) -> std::string {
  absl::string_view trimmed = absl::StripLeadingAsciiWhitespace(str);
  const std::vector<std::string> parts = absl::StrSplit(trimmed, " ");
  return parts[0];
}

auto InsertWordEveryNthPosition(absl::string_view str, absl::string_view word,
                                size_t n) -> std::string {
  std::string buffer;
  int i = 0;
  while (i < str.size()) {
    absl::StrAppend(&buffer, absl::string_view(&str[i], 1));
    ++i;
    if ((i % n) == 0 && i != str.size()) {
      absl::StrAppend(&buffer, word);
    }
  }
  return buffer;
}

}  // namespace bio
