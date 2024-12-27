#include "bio/common/strings.h"

#include <string>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace bio {

auto FirstWord(absl::string_view str) -> std::string {
  absl::string_view trimmed = absl::StripLeadingAsciiWhitespace(str);
  const std::vector<std::string> parts = absl::StrSplit(trimmed, " ");
  return parts[0];
}

}  // namespace bio
