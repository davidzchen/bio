package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fasta-parser",
    srcs = ["fasta-parser.cc"],
    hdrs = ["fasta-parser.h"],
    deps = [
        "//bio/common:line-parser-base",
        "//bio/common:sequence",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/memory",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abxl//abxl/file",
        "@abxl//abxl/status:status_macros",
    ],
)

cc_test(
    name = "fasta-parser_test",
    srcs = ["fasta-parser_test.cc"],
    data = ["//bio/fasta/testdata"],
    deps = [
        ":fasta-parser",
        "//bio/common:sequence",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
