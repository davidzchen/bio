#include "bio/mrf/mrf.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace bio {
namespace {

static constexpr char kColumnNameAlignmentBlocks[] = "AlignmentBlocks";
static constexpr char kColumnNameSequence[] = "Sequence";
static constexpr char kColumnNameQualityScores[] = "QualityScores";
static constexpr char kColumnNameQueryId[] = "QueryId";

}  // namespace

auto MrfHeader::Parse(absl::string_view line)
    -> absl::StatusOr<std::unique_ptr<MrfHeader>> {
  std::vector<std::string> parts = absl::StrSplit(line, "\t");
  std::vector<MrfColumn> columns;
  absl::flat_hash_set<MrfColumn> present_columns;
  for (const auto& part : parts) {
    MrfColumn column;
    if (part == kColumnNameAlignmentBlocks) {
      column = MrfColumn::kAlignmentBlocks;
    } else if (part == kColumnNameSequence) {
      column = MrfColumn::kSequence;
    } else if (part == kColumnNameQualityScores) {
      column = MrfColumn::kQualityScores;
    } else if (part == kColumnNameQueryId) {
      column = MrfColumn::kQueryId;
    } else {
      return absl::InvalidArgumentError(
          absl::StrFormat("Invalid column name: %s", part));
    }
    if (present_columns.contains(column)) {
      return absl::InvalidArgumentError(
          absl::StrFormat("Duplicate column %s found", part));
    }
    present_columns.insert(column);
    columns.push_back(column);
  }

  if (!present_columns.contains(MrfColumn::kAlignmentBlocks)) {
    return absl::InvalidArgumentError(
        "Required AlignmentBlocks column not present");
  }

  return std::make_unique<MrfHeader>(columns);
}

auto MrfHeader::AddComments(const std::vector<std::string>& comments) -> void {
  for (const auto& comment : comments) {
    comments_.push_back(comment);
  }
}

auto MrfHeader::comments() const -> const std::vector<std::string>& {
  return comments_;
}

auto MrfHeader::HasColumn(MrfColumn column) const -> bool {
  return present_columns_.contains(column);
}

auto MrfHeader::AddColumn(MrfColumn column) -> void {
  if (present_columns_.contains(column)) {
    return;
  }
  columns_.push_back(column);
  present_columns_.insert(column);
}

auto MrfHeader::columns() const -> const std::vector<MrfColumn>& {
  return columns_;
}

auto MrfRead::Length() -> size_t {
  int length = 0;
  for (const auto& block : blocks) {
    length += (block.target_end - block.target_start + 1);
  }
  return length;
}

}  // namespace bio
