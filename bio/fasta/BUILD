package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fasta",
    hdrs = ["fasta.h"],
    deps = [
        "//bio/common:strings",
        "@abseil-cpp//absl/strings:str_format",
    ],
)

cc_test(
    name = "fasta_test",
    srcs = ["fasta_test.cc"],
    deps = [
        ":fasta",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "fasta-parser",
    srcs = ["fasta-parser.cc"],
    hdrs = ["fasta-parser.h"],
    deps = [
        ":fasta",
        "//bio/common:line-parser-base",
        "//bio/common:strings",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@gxl//gxl/file",
        "@gxl//gxl/status:status_macros",
    ],
)

cc_test(
    name = "fasta-parser_test",
    srcs = ["fasta-parser_test.cc"],
    data = ["//bio/fasta/testdata"],
    deps = [
        ":fasta",
        ":fasta-parser",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "fasta-writer",
    hdrs = ["fasta-writer.h"],
    deps = [
        ":fasta",
        "//bio/common:entry-writer-base",
        "@abseil-cpp//absl/base:nullability",
        "@gxl//gxl/file",
    ],
)

cc_test(
    name = "fasta-writer_test",
    srcs = ["fasta-writer_test.cc"],
    deps = [
        ":fasta",
        ":fasta-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
        "@gxl//gxl/file:path",
    ],
)
