package(default_visibility = ["//visibility:public"])

cc_library(
    name = "seq",
    srcs = ["seq.cc"],
    hdrs = ["seq.h"],
)

cc_library(
    name = "fasta-parser",
    srcs = ["fasta-parser.cc"],
    hdrs = ["fasta-parser.h"],
    deps = [
        ":seq",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@primrose//prim/file",
    ],
)
