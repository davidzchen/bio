#include "bio/fastq/fastq-parser.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "absl/strings/strip.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/common/strings.h"

namespace bio {
namespace {

namespace file = abxl::file;

static constexpr char kIdentifierPrefix[] = "@";
static constexpr char kQualityIdPrefix[] = "+";

}  // namespace

auto FastqParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastqParser>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<FastqParser>(file);
}

auto FastqParser::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastqParser> {
  absl::StatusOr<std::unique_ptr<FastqParser>> parser = New(path);
  CHECK_OK(parser.status());
  return std::move(parser.value());
}

auto FastqParser::NextSequence(bool truncate_name)
    -> absl::StatusOr<std::unique_ptr<FastqSequence>> {
  if (it_ == file_lines_.end()) {
    return nullptr;
  }

  auto sequence = std::make_unique<FastqSequence>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (!absl::StartsWith(*line, kIdentifierPrefix)) {
      continue;
    }

    // Read sequence identifier line.
    const std::string name = truncate_name ? FirstWord(*line) : *line;
    sequence->name = absl::StripPrefix(name, kIdentifierPrefix);

    // Read sequence line.
    std::optional<std::string> sequence_line = NextLine();
    if (!sequence_line.has_value()) {
      return absl::InvalidArgumentError("Expected sequence line but got EOF");
    }
    sequence->sequence = std::move(sequence_line.value());

    // Read sequence name line.
    std::optional<std::string> quality_id_line = NextLine();
    if (!quality_id_line.has_value()) {
      return absl::InvalidArgumentError("Expected quality ID line but got EOF");
    }
    if (!absl::StartsWith(*quality_id_line, kQualityIdPrefix)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Expected quality ID: '%s' or '+%s'",
                          kQualityIdPrefix, sequence->name));
    }

    // Read quality line.
    std::optional<std::string> quality_line = NextLine();
    if (!quality_line.has_value()) {
      return absl::InvalidArgumentError("Expected quality line but got EOF");
    }
    sequence->quality = std::move(quality_line.value());
    if (sequence->quality.size() != sequence->sequence.size()) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Sequence line length %d does not match quality line length %d",
          sequence->sequence.size(), sequence->quality.size()));
    }

    break;
  }
  return sequence;
}

auto FastqParser::ReadAllSequences(bool truncate_name)
    -> absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>> {
  std::vector<std::unique_ptr<FastqSequence>> sequences;
  while (!eof()) {
    ASSIGN_OR_RETURN(std::optional<std::unique_ptr<FastqSequence>> sequence,
                     NextSequence(truncate_name));
    sequences.push_back(std::move(sequence.value()));
  }
  return sequences;
}

}  // namespace bio
