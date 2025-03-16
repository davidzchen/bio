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
