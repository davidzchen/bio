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

  // Returns whether the end of file has been reached.
  auto eof() const -> bool { return it_ == file_lines_.end(); }

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

  abxl::File* file_;
  abxl::FileLines file_lines_;
  abxl::FileLineIterator it_;
  std::queue<std::string> saved_lines_;
};

}  // namespace bio

#endif  // BIO_COMMON_LINE_PARSER_BASE_H_
