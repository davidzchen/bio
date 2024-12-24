#include "bio/fasta-parser.h"

#include <memory>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "prim/file/file.h"

namespace bio {

absl::StatusOr<std::unique_ptr<FastaParser>> FastaParser::New(
    absl::string_view path) {
  return absl::UnimplementedError("Implement this");
}

absl::StatusOr<Seq*> FastaParser::NextSequence() {
  return absl::UnimplementedError("Implement this");
}

absl::StatusOr<std::vector<Seq>> FastaParser::ReadAllSequences() {
  return absl::UnimplementedError("Implement this");
}

}  // namespace bio
