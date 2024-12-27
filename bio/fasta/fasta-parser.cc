#include "bio/fasta/fasta-parser.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "absl/status/statusor.h"
#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/common/sequence.h"
#include "bio/common/strings.h"

namespace bio {
namespace {

namespace file = abxl::file;

}  // namespace

auto FastaParser::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastaParser>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "r", &file, file::Defaults()));
  return std::make_unique<FastaParser>(file);
}

auto FastaParser::NextSequence(bool truncate_name)
    -> std::optional<std::unique_ptr<Sequence>> {
  if (it_ == file_lines_.end()) {
    return std::nullopt;
  }

  int count = 0;
  auto* sequence = new Sequence();
  std::string buffer;
  for (std::optional<std::string> line = NextLine(); line.has_value();
       line = NextLine()) {
    if (line->empty()) {
      continue;
    }

    if (absl::StartsWith(*line, ">")) {
      ++count;
      if (count == 1) {
        // Save the description line.
        sequence->name = truncate_name ? FirstWord(*line) : *line;
        continue;
      } else if (count == 2) {
        // The second occurrence we find a line starting with ">". This
        // denotes that a new Sequence is starting.
        sequence->sequence = buffer;
        sequence->size = buffer.size();
        PutBack(*line);
        return absl::WrapUnique(sequence);
      }
    }
    absl::StrAppend(&buffer, *line);
  }

  sequence->sequence = buffer;
  sequence->size = buffer.size();
  return absl::WrapUnique(sequence);
}

auto FastaParser::ReadAllSequences(bool truncate_name)
    -> std::vector<std::unique_ptr<Sequence>> {
  std::vector<std::unique_ptr<Sequence>> sequences;
  while (true) {
    std::optional<std::unique_ptr<Sequence>> sequence =
        NextSequence(truncate_name);
    if (!sequence.has_value()) {
      break;
    }
    sequences.push_back(std::move(sequence.value()));
  }
  return sequences;
}

}  // namespace bio
