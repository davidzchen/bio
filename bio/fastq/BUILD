package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fastq-parser",
    srcs = ["fastq-parser.cc"],
    hdrs = ["fastq-parser.h"],
    deps = [
        "//bio/common:line-parser-base",
        "//bio/common:strings",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@abxl//abxl/file",
        "@abxl//abxl/status:status_macros",
    ],
)

cc_test(
    name = "fastq-parser_test",
    srcs = ["fastq-parser_test.cc"],
    data = ["//bio/fastq/testdata"],
    deps = [
        ":fastq-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
