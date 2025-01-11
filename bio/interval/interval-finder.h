#ifndef BIO_INTERVAL_INTERVAL_FINDER_H_
#define BIO_INTERVAL_INTERVAL_FINDER_H_

#include <string>
#include <vector>

#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "bio/interval/interval.h"

namespace bio {

// Finds overlapping intervals.
class IntervalFinder {
 public:
  IntervalFinder() = default;
  ~IntervalFinder() = default;

  // Adds intervals from the given file to the search space.
  auto AddIntervalsFromFile(absl::string_view file_name) -> absl::Status;

  // Adds the provided intervals.
  auto AddIntervals(const std::vector<Interval>& intervals) -> void;

  // Gets the intervals that overlap with the query interval specified by the
  // given chromosome, start, and end.
  auto OverlappingIntervals(absl::string_view chromosome, size_t start,
                            size_t end) -> std::vector<Interval>;

  // Returns all intervals that have been added.
  auto Intervals() -> std::vector<Interval>;
};

}  // namespace bio

#endif  // BIO_INTERVAL_INTERVAL_FINDER_H_
