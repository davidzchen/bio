#ifndef BIO_SEQ_H_
#define BIO_SEQ_H_

#include <cstdint>
#include <string>

namespace bio {

// Represents a sequence.
struct Seq {
  // Name of sequnce.
  std::string name;

  // Sequence base by base.
  std::string sequence;

  // Size of sequence.
  int size;

  // Repeat mask (optional)
  uint8_t* mask;
};

}  // namespace bio

#endif  // BIO_SEQ_H_
