#ifndef BIO_INTERVAL_INTERVAL_H_
#define BIO_INTERVAL_INTERVAL_H_

#include <cstdlib>
#include <string>
#include <vector>

#include "bio/common/sequence.h"

namespace bio {

// Represents a sub-interval.
struct SubInterval {
  // The start of the sub-interval.
  size_t start;

  // The end of the sub-interval.
  size_t end;
};

// Represents an Interval entry.
struct Interval {
  // TODO: Figure out what this is.
  int source;

  // The name of the interval.
  std::string name;

  // The chromosome.
  std::string chromosome;

  // The strand the interval corresponds to.
  Strand strand;

  // The interval start.
  size_t start;

  // The interval end.
  size_t end;

  // Vector of sub-intervals.
  std::vector<SubInterval> sub_intervals;
};

}  // namespace bio

#endif  // BIO_INTERVAL_INTERVAL_H_
