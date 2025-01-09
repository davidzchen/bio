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

#include "bio/mrf/mrf-writer.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/nullability.h"
#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/string_view.h"
#include "bio/mrf/mrf.h"
#include "gxl/file/file.h"
#include "gxl/status/status_macros.h"

namespace bio {
namespace {

namespace file = ::gxl::file;

}  // namespace

auto MrfWriter::New(absl::string_view path, absl::Nonnull<MrfHeader*> header)
    -> absl::StatusOr<std::unique_ptr<MrfWriter>> {
  if (header == nullptr) {
    return absl::InvalidArgumentError("header cannot be null");
  }
  gxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "w", &file, file::Defaults()));
  return std::make_unique<MrfWriter>(file, header);
}

auto MrfWriter::NewOrDie(absl::string_view path,
                         absl::Nonnull<MrfHeader*> header)
    -> std::unique_ptr<MrfWriter> {
  absl::StatusOr<std::unique_ptr<MrfWriter>> writer = New(path, header);
  CHECK_OK(writer.status());
  return std::move(writer.value());
}

auto MrfWriter::Start() -> absl::Status {
  CHECK_NE(file_, nullptr);
  CHECK_NE(header_, nullptr);

  // Write comments.
  for (const auto& comment : header_->comments()) {
    const std::string comment_line = absl::StrCat("# ", comment, "\n");
    size_t size = file_->WriteString(comment_line);
    if (size != comment_line.size()) {
      return absl::DataLossError(
          absl::StrFormat("Expected to write %d bytes but wrote %d",
                          comment_line.size(), size));
    }
  }

  // Write header line.
  const std::string header_str = absl::StrCat(header_->string(), "\n");
  size_t size = file_->WriteString(header_str);
  if (size != header_str.size()) {
    return absl::DataLossError(absl::StrFormat(
        "Expected to write %d bytes but wrote %d", header_str.size(), size));
  }

  started_ = true;
  return absl::OkStatus();
}

auto MrfWriter::Write(const std::vector<MrfEntry>& entries) -> absl::Status {
  for (const auto& entry : entries) {
    RETURN_IF_ERROR(Write(entry));
  }
  return absl::OkStatus();
}

namespace {

auto MarshalAlignmentBlocks(absl::Nonnull<const MrfEntry*> entry)
    -> std::string {
  if (entry->is_paired_end) {
    const std::string read1_blocks = entry->read1.AlignmentBlocksStr();
    const std::string read2_blocks = entry->read2.AlignmentBlocksStr();
    return absl::StrFormat("%s|%s", read1_blocks, read2_blocks);
  } else {
    return entry->read1.AlignmentBlocksStr();
  }
}

auto MarshalSequences(absl::Nonnull<const MrfEntry*> entry) -> std::string {
  if (entry->is_paired_end) {
    const std::string read1_sequence = entry->read1.sequence;
    const std::string read2_sequence = entry->read2.sequence;
    return absl::StrFormat("%s|%s", read1_sequence, read2_sequence);
  } else {
    return entry->read1.sequence;
  }
}

auto MarshalQualityScores(absl::Nonnull<const MrfEntry*> entry) -> std::string {
  if (entry->is_paired_end) {
    const std::string read1_quality_scores = entry->read1.quality_scores;
    const std::string read2_quality_scores = entry->read2.quality_scores;
    return absl::StrFormat("%s|%s", read1_quality_scores, read2_quality_scores);
  } else {
    return entry->read1.quality_scores;
  }
}
auto MarshalQueryId(absl::Nonnull<const MrfEntry*> entry) -> std::string {
  if (entry->is_paired_end) {
    const std::string read1_query_id = entry->read1.query_id;
    const std::string read2_query_id = entry->read2.query_id;
    return absl::StrFormat("%s|%s", read1_query_id, read2_query_id);
  } else {
    return entry->read1.query_id;
  }
}

}  // namespace

auto MrfWriter::Write(absl::Nonnull<const MrfEntry*> entry) -> absl::Status {
  if (!started_) {
    return absl::FailedPreconditionError(
        "Start() must be called before Write()");
  }
  if (absl::Status status = entry->Validate(*header_); !status.ok()) {
    return status;
  }

  std::vector<std::string> parts;
  for (const auto& column : header_->columns()) {
    switch (column) {
      case MrfColumn::kAlignmentBlocks:
        parts.push_back(MarshalAlignmentBlocks(entry));
        break;
      case MrfColumn::kSequence:
        parts.push_back(MarshalSequences(entry));
        break;
      case MrfColumn::kQualityScores:
        parts.push_back(MarshalQualityScores(entry));
        break;
      default:  // case MrfColumn::kQueryId:
        parts.push_back(MarshalQueryId(entry));
        break;
    }
  }

  if (parts.size() != header_->columns().size()) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Number of columns found %d does not match number of header columns %d",
        parts.size(), header_->columns().size()));
  }

  const std::string marshalled_entry =
      absl::StrCat(absl::StrJoin(parts, "\t"), "\n");
  size_t size = file_->WriteString(marshalled_entry);
  if (size != marshalled_entry.size()) {
    return absl::DataLossError(
        absl::StrFormat("Expected to write %d bytes but wrote %d",
                        marshalled_entry.size(), size));
  }
  return absl::OkStatus();
}

auto MrfWriter::Write(const MrfEntry& entry) -> absl::Status {
  return Write(&entry);
}

auto MrfWriter::Close() -> absl::Status {
  if (!started_) {
    return absl::FailedPreconditionError(
        "Start() must be called before Close()");
  }
  return file_->Close(file::Defaults());
}

}  // namespace bio
