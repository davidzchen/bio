#ifndef BIO_COMMON_STRINGS_H_
#define BIO_COMMON_STRINGS_H_

#include <cstdlib>
#include <string>

#include "absl/strings/string_view.h"

namespace bio {

// Returns the first word in the string.
auto FirstWord(absl::string_view str) -> std::string;

// Inserts the given word at every nth position in the string, but not at the
// end.
auto InsertWordEveryNthPosition(absl::string_view str, absl::string_view word,
                                size_t n) -> std::string;

}  // namespace bio

#endif  // BIO_COMMON_STRINGS_H_
