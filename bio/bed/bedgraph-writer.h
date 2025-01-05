#ifndef BIO_BED_BEDGRAPH_WriTER_H_
#define BIO_BED_BEDGRAPH_WriTER_H_

#include "absl/base/nullability.h"
#include "abxl/file/file.h"
#include "bio/bed/bedgraph.h"
#include "bio/common/entry-writer-base.h"

namespace bio {

// Writer for BedGraph files.
//
// Example usage:
//
// ```
// absl::StatusOr<std::unique_ptr<BedGraphWriter>> writer_or =
//     BedWriter::New("path/to/out.bedgraph");
// if (!writer_or.ok()) {
//   // Handle error.
// }
// std::unique_ptr<BedGraphWriter> writer = std::move(writer_or.value());
//
// std::vector<BedGraphEntry> entries = {/*BED entries*/};
// absl::Status status = writer->Write(entry);
// if (!status.ok()) {
//   // Handle error.
// }
//
// BedGraphEntry entry = {/*BED entry*/};
// status = writer->Write(entry);
// if (!status.ok()) {
//   // Handle error.
// }
// ```
//
// Or if using status macros:
//
// ```
// ASSIGN_OR_RETURN(std::unique_ptr<BedGraphWriter> writer,
//                  BedWriter::New("path/to/out.fastq"));
// std::vector<BedGraphEntry> entries = {/*BED entries*/};
// RETURN_IF_ERROR(writer->Write(entries));
//
// BedGraphEntry entry = {/*BED entry*/};
// RETURN_IF_ERROR(writer->Write(entry));
// ```
class BedGraphWriter : public EntryWriterBase<BedGraphWriter, BedGraphEntry> {
 public:
  explicit BedGraphWriter(absl::Nonnull<abxl::File*> file)
      : EntryWriterBase(file) {}
};

}  // namespace bio

#endif  // BIO_BED_BEDGRAPH_WriTER_H_
