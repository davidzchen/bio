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

#include <memory>

#include "absl/status/status_matchers.h"
#include "absl/status/statusor.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace bio::internal {
namespace {

using ::absl_testing::IsOk;
using ::absl_testing::StatusIs;
using ::testing::ElementsAre;
using ::testing::HasSubstr;

TEST(CigarLexer, Empty) {
  CigarLexer lexer("");
  absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
  EXPECT_THAT(token.status(), IsOk());
  EXPECT_EQ(*token, nullptr);
}

auto TokensEqual(const CigarToken* actual, const CigarToken* expected) {
  EXPECT_EQ(actual->text(), expected->text());
  EXPECT_EQ(actual->token_type(), expected->token_type());
}

TEST(CigarLexer, SingleLength) {
  CigarLexer lexer("12");
  absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
  EXPECT_THAT(token.status(), IsOk());
  ASSERT_NE(*token, nullptr);
  TokensEqual(token->get(), CigarLengthToken::New("12").get());
}

TEST(CigarLexer, SingleOperation) {
  CigarLexer lexer("M");
  absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
  EXPECT_THAT(token.status(), IsOk());
  ASSERT_NE(*token, nullptr);
  TokensEqual(token->get(), CigarOperationToken::New("M").get());
}

TEST(CigarLexer, MultipleOperations) {
  CigarLexer lexer("3M2I356M");

  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("3").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("M").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("2").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("I").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("356").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("M").get());
  }
}

TEST(CigarLexer, MultipleOperationsRepeatedOperations) {
  CigarLexer lexer("3M2IM36D");

  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("3").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("M").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("2").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("I").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("M").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarLengthToken::New("36").get());
  }
  {
    absl::StatusOr<std::unique_ptr<CigarToken>> token = lexer.Next();
    EXPECT_THAT(token.status(), IsOk());
    ASSERT_NE(*token, nullptr);
    TokensEqual(token->get(), CigarOperationToken::New("D").get());
  }
}

}  // namespace
}  // namespace bio::internal
