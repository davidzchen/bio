#ifndef BIO_SEQUENCE_H_
#define BIO_SEQUENCE_H_

#include <string>

namespace bio {

// Represents a sequence.
struct Sequence {
  // Name of sequnce.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // Size of sequence.
  int size;
};

}  // namespace bio

#endif  // BIO_SEQUENCE_H_
