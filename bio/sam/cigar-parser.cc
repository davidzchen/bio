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

#include "bio/sam/cigar-parser.h"

#include <memory>
#include <optional>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "bio/sam/cigar.h"
#include "bio/sam/internal/cigar-lexer.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

using ::bio::internal::CigarLengthToken;
using ::bio::internal::CigarLexer;
using ::bio::internal::CigarOperationToken;
using ::bio::internal::CigarToken;
using ::bio::internal::CigarTokenType;

}  // namespace

auto CigarParser::Parse(absl::string_view cigar) -> absl::StatusOr<Cigar> {
  CigarLexer lexer(cigar);
  std::vector<CigarOperation> operations;
  std::optional<CigarOperation> operation;
  while (true) {
    ASSIGN_OR_RETURN(std::unique_ptr<CigarToken> token, lexer.Next());
    if (token == nullptr) {
      break;
    }

    if (token->token_type() == CigarTokenType::kOperation) {
      const CigarOperationToken* op_token =
          dynamic_cast<CigarOperationToken*>(token.get());
      CigarType type = op_token->type();
      if (type == CigarType::kInvalid) {
        return absl::InvalidArgumentError(
            absl::StrFormat("Invalid cigar operation type: %s", token->text()));
      }
      if (operation == std::nullopt) {
        return absl::InvalidArgumentError(absl::StrFormat(
            "Failed to parse CIGAR string: '%s': consecutive operations found",
            cigar));
      }
      operation->type = type;
      operations.push_back(*operation);
      operation = std::nullopt;

    } else if (token->token_type() == CigarTokenType::kLength) {
      const CigarLengthToken* length_token =
          dynamic_cast<CigarLengthToken*>(token.get());
      if (operation.has_value()) {
        return absl::InternalError(absl::StrFormat(
            "Failed to parse CIGAR string: '%s': consecutive lengths found",
            cigar));
      }
      operation = {.length = length_token->length()};
    }
  }
  return Cigar{.operations = operations};
}

auto ParseCigarOrDie(absl::string_view cigar_str) -> Cigar {
  CigarParser parser;
  absl::StatusOr<Cigar> cigar = parser.Parse(cigar_str);
  CHECK_OK(cigar.status()) << "Failed to parse CIGAR string: " << cigar_str;
  return *cigar;
}

}  // namespace bio
