#include "bio/fastq/fastq-parser.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/common/sequence.h"

namespace bio {

namespace file = abxl::file;

auto FastqParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastqParser>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<FastqParser>(file);
}

namespace {

auto MaybeTruncateName(absl::string_view line, bool truncate_name)
    -> std::string {
  if (!truncate_name) {
    return std::string(line);
  }
  absl::string_view trimmed = absl::StripLeadingAsciiWhitespace(line);
  const std::vector<std::string> parts = absl::StrSplit(trimmed, " ");
  return parts[0];
}

}  // namespace

auto FastqParser::NextLine() -> std::optional<std::string> {
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

auto FastqParser::PutBack(std::string line) -> void { saved_lines_.push(line); }

auto FastqParser::NextSequence(bool truncate_name)
    -> absl::StatusOr<std::unique_ptr<FastqSequence>> {
  if (it_ == file_lines_.end()) {
    return absl::ResourceExhaustedError("Reached EOF");
  }

  auto fastq_sequence = std::make_unique<FastqSequence>();
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }
    if (!absl::StartsWith(*line, "@")) {
      continue;
    }

    // Read sequence identifier line.
    fastq_sequence->sequence.name = MaybeTruncateName(*line, truncate_name);

    // Read sequence line.
    std::optional<std::string> sequence_line = NextLine();
    if (!sequence_line.has_value()) {
      return absl::InvalidArgumentError("Expected sequence line but got EOF");
    }
    fastq_sequence->sequence.sequence = std::move(sequence_line.value());
    fastq_sequence->sequence.size = fastq_sequence->sequence.sequence.size();

    // Read sequence name line.
    std::optional<std::string> quality_id_line = NextLine();
    if (!quality_id_line.has_value()) {
      return absl::InvalidArgumentError("Expected quality ID line but got EOF");
    }
    if (!absl::StartsWith(*quality_id_line, "+")) {
      return absl::InvalidArgumentError(absl::StrFormat(
          "Expected quality ID: '+' or '+%s'", fastq_sequence->sequence.name));
    }

    // Read quality line.
    std::optional<std::string> quality_line = NextLine();
    if (!quality_line.has_value()) {
      return absl::InvalidArgumentError("Expected quality line but got EOF");
    }
    fastq_sequence->quality = std::move(quality_line.value());
    break;
  }
  return fastq_sequence;
}

auto FastqParser::ReadAllSequences(bool truncate_name)
    -> absl::StatusOr<std::vector<std::unique_ptr<FastqSequence>>> {
  std::vector<std::unique_ptr<FastqSequence>> sequences;
  while (true) {
    ASSIGN_OR_RETURN(std::optional<std::unique_ptr<FastqSequence>> sequence,
                     NextSequence(truncate_name));
    if (!sequence.has_value()) {
      break;
    }
    sequences.push_back(std::move(sequence.value()));
  }
  return sequences;
}

}  // namespace bio