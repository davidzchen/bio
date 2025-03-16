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

#include "bio/sam/internal/cigar-lexer.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "absl/strings/numbers.h"
#include "absl/strings/string_view.h"
#include "bio/sam/cigar.h"

namespace bio::internal {

auto operator<<(std::ostream& os, CigarTokenType type) -> std::ostream& {
  switch (type) {
    case CigarTokenType::kOperation:
      os << "kOperation";
      break;
    case CigarTokenType::kLength:
      os << "kLength";
      break;
    default:
      os << "kInvalid";
      break;
  }
  return os;
}

namespace {

auto ParseCigarType(absl::string_view op) -> CigarType {
  if (op.length() != 1) {
    return CigarType::kInvalid;
  }
  if (op == "M") {
    return CigarType::kAlignmentMatch;
  } else if (op == "I") {
    return CigarType::kInsertion;
  } else if (op == "D") {
    return CigarType::kDeletion;
  } else if (op == "N") {
    return CigarType::kSkippedRegion;
  } else if (op == "S") {
    return CigarType::kSoftClipping;
  } else if (op == "H") {
    return CigarType::kHardClipping;
  } else if (op == "P") {
    return CigarType::kPadding;
  } else if (op == "=") {
    return CigarType::kSequenceMatch;
  } else if (op == "X") {
    return CigarType::kSequenceMismatch;
  } else {
    return CigarType::kInvalid;
  }
}

}  // namespace

CigarOperationToken::CigarOperationToken(absl::string_view text)
    : CigarToken(text), type_(ParseCigarType(text)) {}

auto CigarOperationToken::New(absl::string_view text)
    -> std::unique_ptr<CigarOperationToken> {
  return std::make_unique<CigarOperationToken>(text);
}

namespace {

auto ParseLength(absl::string_view text) {
  size_t length = 0;
  CHECK(absl::SimpleAtoi(text, &length));  // Crash OK
  return length;
}

}  // namespace

CigarLengthToken::CigarLengthToken(absl::string_view text)
    : CigarToken(text), length_(ParseLength(text)) {}

auto CigarLengthToken::New(absl::string_view text)
    -> std::unique_ptr<CigarLengthToken> {
  return std::make_unique<CigarLengthToken>(text);
}

CigarLexer::CigarLexer(absl::string_view cigar)
    : cigar_(std::string(cigar)),
      start_cursor_(0),
      lookahead_cursor_(0),
      in_token_(std::nullopt) {}

auto CigarLexer::TokenType() -> CigarTokenType {
  CigarTokenType token_type = CigarTokenType::kOperation;
  if (std::isdigit(cigar_[lookahead_cursor_])) {
    token_type = CigarTokenType::kLength;
  }
  return token_type;
}

auto CigarLexer::GetStringAndAdvance() -> absl::string_view {
  size_t len = lookahead_cursor_ - start_cursor_;
  size_t start = start_cursor_;
  start_cursor_ = lookahead_cursor_;
  return absl::string_view(&cigar_[start], len);
}

auto CigarLexer::Next() -> absl::StatusOr<std::unique_ptr<CigarToken>> {
  if (cigar_.empty()) {
    return nullptr;
  }

  while (lookahead_cursor_ < cigar_.size()) {
    CigarTokenType token_type = TokenType();
    if (lookahead_cursor_ == 0) {
      in_token_ = token_type;
      ++lookahead_cursor_;
      token_type = TokenType();
    }

    CHECK(in_token_.has_value());  // Crash OK
    switch (token_type) {
      case CigarTokenType::kOperation: {
        CigarTokenType in_token = in_token_.value();
        in_token_ = CigarTokenType::kOperation;

        absl::string_view text = GetStringAndAdvance();
        ++lookahead_cursor_;
        if (in_token == CigarTokenType::kLength) {
          return std::make_unique<CigarLengthToken>(text);
        }
        return std::make_unique<CigarOperationToken>(text);
        break;
      }
      case CigarTokenType::kLength: {
        CigarTokenType in_token = in_token_.value();
        in_token_ = CigarTokenType::kLength;
        if (in_token == CigarTokenType::kOperation) {
          absl::string_view text = GetStringAndAdvance();
          ++lookahead_cursor_;
          return std::make_unique<CigarOperationToken>(text);
        }
        ++lookahead_cursor_;
        break;
      }
      default:
        CHECK(false) << "Should not be reached.";  // Crash OK
        break;
    }
  }

  if (start_cursor_ < lookahead_cursor_) {
    absl::string_view text = GetStringAndAdvance();
    if (in_token_ == CigarTokenType::kLength) {
      return std::make_unique<CigarLengthToken>(text);
    }
    return std::make_unique<CigarOperationToken>(text);
  }

  return nullptr;
}

}  // namespace bio::internal
