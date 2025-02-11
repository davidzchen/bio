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

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <limits>
#include <optional>
#include <queue>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "bio/common/sequence.h"
#include "gxl/file/file.h"
#include "gxl/file/filelineiter.h"

namespace bio {

// Base class for parsers that read a file line by line. This class provides
// methods for reading lines as well as putting lines back to read at a later
// point.
class LineParserBase {
 public:
  explicit LineParserBase(absl::Nonnull<gxl::File*> file)
      : file_(file),
        file_lines_(gxl::FileLines(file_->filename(), file_)),
        it_(file_lines_.begin()),
        line_number_(0) {}

  // Returns whether the end of file has been reached.
  auto eof() const -> bool { return it_ == file_lines_.end(); }

 protected:
  // Returns the next line or std::nullopt if EOF has been reached. If there are
  // saved lines from calls to PutBack(), these lines will be returned first.
  auto NextLine() -> std::optional<std::string> {
    ++line_number_;
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
  auto PutBack(std::string line) -> void {
    --line_number_;
    saved_lines_.push(line);
  }

  // Parses an integer value from the provided str for the specified field.
  template <typename IntType>
  auto ParseInt(absl::string_view str, absl::string_view field)
      -> absl::StatusOr<IntType> {
    IntType value = 0;
    if (!absl::SimpleAtoi(str, &value)) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Invalid %s format: '%s'", line_number_, field, str));
    }
    return value;
  }

  // Parses a uint8_t value from the provided str for the specified field.
  auto ParseUInt8(absl::string_view str, absl::string_view field)
      -> absl::StatusOr<uint8_t> {
    try {
      size_t index = 0;
      int value = std::stoi(std::string(str), &index, 10);
      if (index != str.size()) {
        return absl::OutOfRangeError(
            absl::StrFormat("Line %d: value is out of range for a uint8_t: %s",
                            line_number_, field));
      }

      if (value < 0 || value > std::numeric_limits<uint8_t>::max()) {
        return absl::OutOfRangeError(absl::StrFormat(
            "Line %d: value is out of range for a uint8_t", line_number_));
      }

      return static_cast<uint8_t>(value);
    } catch (const std::exception& e) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Failed to parse '%s': %s", line_number_, field, e.what()));
    }
    return absl::InternalError("Should not be reached");
  }

  // Parses a double value from the provided str for the specified field.
  auto ParseDouble(absl::string_view str, absl::string_view field)
      -> absl::StatusOr<double> {
    double value = 0;
    if (!absl::SimpleAtod(str, &value)) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Invalid %s format: '%s'", line_number_, field, str));
    }
    return value;
  }

  // Parses the strand from the provided str.
  auto ParseStrand(absl::string_view str) -> absl::StatusOr<Strand> {
    static constexpr char kStrandSense[] = "+";
    static constexpr char kStrandAntisense[] = "-";
    if (str == kStrandSense) {
      return Strand::kSense;
    } else if (str == kStrandAntisense) {
      return Strand::kAntisense;
    } else {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Line %d: Invalid strand format: '%s'", line_number_, str));
    }
  }

  gxl::File* file_;
  gxl::FileLines file_lines_;
  gxl::FileLineIterator it_;
  std::queue<std::string> saved_lines_;
  size_t line_number_;
};

}  // namespace bio

#endif  // BIO_COMMON_LINE_PARSER_BASE_H_
