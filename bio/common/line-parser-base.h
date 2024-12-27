#ifndef BIO_COMMON_LINE_PARSER_BASE_H_
#define BIO_COMMON_LINE_PARSER_BASE_H_

#include <optional>
#include <queue>
#include <string>

#include "absl/base/nullability.h"
#include "abxl/file/file.h"
#include "abxl/file/filelineiter.h"

namespace bio {

// Base class for parsers that read a file line by line. This class provides
// methods for reading lines as well as putting lines back to read at a later
// point.
class LineParserBase {
 public:
  explicit LineParserBase(absl::Nonnull<abxl::File*> file)
      : file_(file),
        file_lines_(abxl::FileLines(file_->filename(), file_)),
        it_(file_lines_.begin()) {}

 protected:
  // Returns the next line or std::nullopt if EOF has been reached. If there are
  // saved lines from calls to PutBack(), these lines will be returned first.
  auto NextLine() -> std::optional<std::string> {
    if (!saved_lines_.empty()) {
      std::string line = saved_lines_.front();
      saved_lines_.pop();
      return line;
    }
    if (it_ == file_lines_.end()) {
      return std::nullopt;
    }
    std::string line = *it_;
    ++it_;
    return line;
  }

  // Saves the line in the saved_lines_ queue.
  auto PutBack(std::string line) -> void { saved_lines_.push(line); }

 private:
  abxl::File* file_;
  abxl::FileLines file_lines_;
  abxl::FileLineIterator it_;
  std::queue<std::string> saved_lines_;
};

}  // namespace bio

#endif  // BIO_COMMON_LINE_PARSER_BASE_H_
