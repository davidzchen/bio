#ifndef BIO_FASTQ_FASTQ_H_
#define BIO_FASTQ_FASTQ_H_

#include <cstdlib>
#include <string>

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
};

}  // namespace bio

#endif  // BIO_FASTQ_FASTQ_H_
