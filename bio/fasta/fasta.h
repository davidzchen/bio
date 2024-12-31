#ifndef BIO_FASTA_FASTA_H_
#define BIO_FASTA_FASTA_H_

#include <cstdlib>
#include <string>

#include "absl/strings/str_format.h"
#include "bio/common/strings.h"

namespace bio {

// The number of characters per line when serializing a FASTA sequence to a
// string.
static constexpr size_t kCharsPerLine = 60;

// Contains data from a FASTA file for a single sequence.
struct FastaSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }

  // Serializes the sequence to its string format.
  auto string() const -> std::string {
    std::string seq = InsertWordEveryNthPosition(sequence, "\n", kCharsPerLine);
    return absl::StrFormat(">%s\n%s\n", name, seq);
  }
};

}  // namespace bio

#endif  // BIO_FASTA_FASTA_H_
