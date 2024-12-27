#ifndef BIO_COMMON_STRINGS_H_
#define BIO_COMMON_STRINGS_H_

#include <string>

#include "absl/strings/string_view.h"

namespace bio {

// Returns the first word in the string.
auto FirstWord(absl::string_view str) -> std::string;

}  // namespace bio

#endif  // BIO_COMMON_STRINGS_H_
