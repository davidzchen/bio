#ifndef BIO_SAM_CIGAR_H_
#define BIO_SAM_CIGAR_H_

#include <cstdlib>

namespace bio {

// Represents a CIGAR operation type.
enum class CigarType {
  kAlignmentMatch,    // M: Alignment match
  kInsertion,         // I: Insertion into the reference
  kDeletion,          // D: Deletion from the reference
  kSkippedRegion,     // N: Skipped region from the reference
  kSoftClipping,      // S: Soft clipping
  kHardClipping,      // H: Hard clipping
  kPadding,           // P: Padding
  kSequenceMatch,     // =: Sequence match
  kSequenceMismatch,  // X: Sequence mismatch
  kInvalid
};

// Represents a single CIGAR operation.
struct CigarOperation {
  // The cigar operation type.
  CigarType type;

  // The length of the operation.
  size_t length;

  // Checks for equality.
  auto operator==(const CigarOperation& rhs) const -> bool {
    return type == rhs.type && length == rhs.length;
  }
};

}  // namespace bio

#endif  // BIO_SAM_CIGAR_H_
