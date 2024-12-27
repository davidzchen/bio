#include "bio/common/line-parser-base.h"

#include <optional>
#include <string>

#include "absl/base/nullability.h"
#include "absl/status/status.h"
#include "absl/status/status_matchers.h"
#include "abxl/file/file.h"
#include "gtest/gtest.h"

namespace bio {
namespace {

namespace file = abxl::file;
using ::absl_testing::IsOk;

class LineParserPeer : public LineParserBase {
 public:
  explicit LineParserPeer(absl::Nonnull<abxl::File*> file)
      : LineParserBase(file) {}

  using LineParserBase::NextLine;
  using LineParserBase::PutBack;
};

TEST(LineParserBase, NextLineEmptyFile) {
  abxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/empty", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, std::nullopt);
}

TEST(LineParserBase, NextLine) {
  abxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/lines", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 2");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 3");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 4");

  line = parser.NextLine();
  EXPECT_EQ(line, std::nullopt);
}

TEST(LineParserBase, PutBack) {
  abxl::File* file;
  absl::Status status =
      file::Open("bio/common/testdata/lines", "r", &file, file::Defaults());
  EXPECT_THAT(status, IsOk());

  LineParserPeer parser(file);
  std::optional<std::string> line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  parser.PutBack(*line);
  line = parser.NextLine();
  EXPECT_EQ(line, "line 1");

  line = parser.NextLine();
  EXPECT_EQ(line, "line 2");
}

}  // namespace
}  // namespace bio
