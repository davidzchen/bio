#include "bio/fasta/fasta-writer.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/check.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"
#include "absl/strings/string_view.h"
#include "abxl/file/file.h"
#include "abxl/status/status_macros.h"
#include "bio/fasta/fasta.h"

namespace bio {
namespace {

namespace file = abxl::file;

}  // namespace

auto FastaWriter::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastaWriter>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "w", &file, file::Defaults()));
  return std::make_unique<FastaWriter>(file);
}

auto FastaWriter::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastaWriter> {
  absl::StatusOr<std::unique_ptr<FastaWriter>> writer = New(path);
  CHECK_OK(writer.status());
  return std::move(writer.value());
}

auto FastaWriter::Write(const std::vector<FastaSequence>& sequences)
    -> absl::Status {
  for (const auto& sequence : sequences) {
    RETURN_IF_ERROR(Write(sequence));
  }
  return absl::OkStatus();
}

auto FastaWriter::Write(const FastaSequence& sequence) -> absl::Status {
  const std::string seq = sequence.string();
  size_t size = file_->WriteString(seq);
  if (size != seq.size()) {
    return absl::DataLossError(absl::StrFormat(
        "Expected to write %d bytes but only wrote %d", seq.size(), size));
  }
  return absl::OkStatus();
}

auto FastaWriter::Close() -> absl::Status {
  return file_->Close(file::Defaults());
}

}  // namespace bio
