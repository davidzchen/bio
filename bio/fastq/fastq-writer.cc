#include "bio/fastq/fastq-writer.h"

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
#include "bio/fastq/fastq.h"

namespace bio {
namespace {

namespace file = abxl::file;

}

auto FastqWriter::New(absl::string_view path)
    -> absl::StatusOr<std::unique_ptr<FastqWriter>> {
  abxl::File* file;
  RETURN_IF_ERROR(file::Open(path, "w", &file, file::Defaults()));
  return std::make_unique<FastqWriter>(file);
}

auto FastqWriter::NewOrDie(absl::string_view path)
    -> std::unique_ptr<FastqWriter> {
  absl::StatusOr<std::unique_ptr<FastqWriter>> writer = New(path);
  CHECK_OK(writer.status());
  return std::move(writer.value());
}

auto FastqWriter::Write(const std::vector<FastqSequence>& sequences)
    -> absl::Status {
  for (const auto& sequence : sequences) {
    RETURN_IF_ERROR(Write(sequence));
  }
  return absl::OkStatus();
}

auto FastqWriter::Write(const FastqSequence& sequence) -> absl::Status {
  const std::string seq = sequence.string();
  size_t size = file_->WriteString(seq);
  if (size != seq.size()) {
    return absl::DataLossError(absl::StrFormat(
        "Expected to write %d bytes but only wrote %d", seq.size(), size));
  }
  return absl::OkStatus();
}

auto FastqWriter::Close() -> absl::Status {
  return file_->Close(file::Defaults());
}

}  // namespace bio
