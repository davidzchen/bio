#ifndef BIO_FASTQ_FASTQ_H_
#define BIO_FASTQ_FASTQ_H_

#include <cstdlib>
#include <string>

#include "absl/strings/str_format.h"

namespace bio {

// Contains data from a FASTQ file for a single sequence.
struct FastqSequence {
  // The sequence name.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // The quality values for the sequence.
  std::string quality;

  // Returns the size of the sequence.
  auto size() const -> size_t { return sequence.size(); }

  // Serializes the sequence to its string format.
  auto string() const -> std::string {
    return absl::StrFormat("@%s\n%s\n+\n%s\n", name, sequence, quality);
  }
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_H_
