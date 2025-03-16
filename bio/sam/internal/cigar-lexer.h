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

#ifndef BIO_SAM_INTERNAL_CIGAR_ITERATOR_H_
#define BIO_SAM_INTERNAL_CIGAR_ITERATOR_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "bio/sam/cigar.h"

namespace bio::internal {

enum class CigarTokenType {
  kOperation,
  kLength,
};

// std::ostream << overload for CigarTokenType.
auto operator<<(std::ostream& os, CigarTokenType type) -> std::ostream&;

class CigarToken {
 public:
  explicit CigarToken(absl::string_view text) : text_(text) {}
  virtual ~CigarToken() = default;

  // Returns the value.
  auto text() const -> absl::string_view { return text_; }

  // Returns the token type.
  virtual auto token_type() const -> CigarTokenType = 0;

 private:
  std::string text_;
};

class CigarOperationToken : public CigarToken {
 public:
  explicit CigarOperationToken(absl::string_view text);

  // Constructs a new CigarOperationToken.
  static auto New(absl::string_view text)
      -> std::unique_ptr<CigarOperationToken>;

  // Returns the CigarType.
  auto type() const -> CigarType { return type_; }

  // Returns the token type.
  auto token_type() const -> CigarTokenType override {
    return CigarTokenType::kOperation;
  }

 private:
  CigarType type_;
};

class CigarLengthToken : public CigarToken {
 public:
  explicit CigarLengthToken(absl::string_view text);

  // Constructs a new CigarOperationToken.
  static auto New(absl::string_view text) -> std::unique_ptr<CigarLengthToken>;

  // Returns the length.
  auto length() const -> size_t { return length_; }

  // Returns the token type.
  auto token_type() const -> CigarTokenType override {
    return CigarTokenType::kLength;
  }

 private:
  size_t length_;
};

// Lexer for CIGAR strings.
class CigarLexer {
 public:
  explicit CigarLexer(absl::string_view cigar);

  // Returns the next token.
  auto Next() -> absl::StatusOr<std::unique_ptr<CigarToken>>;

 private:
  auto GetStringAndAdvance() -> absl::string_view;

  // Returns the token type for the character that lookahead_cursor_ is curently
  // pointing to.
  auto TokenType() -> CigarTokenType;

  std::string cigar_;
  absl::flat_hash_set<char> separators_;
  size_t start_cursor_;
  size_t lookahead_cursor_;
  std::optional<CigarTokenType> in_token_;
};

}  // namespace bio::internal

#endif  // BIO_SAM_INTERNAL_CIGAR_ITERATOR_H_
