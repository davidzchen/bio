package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fastq",
    hdrs = ["fastq.h"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "fastq_test",
    srcs = ["fastq_test.cc"],
    deps = [
        ":fastq",
        "@abseil-cpp//absl/strings:str_format",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "fastq-parser",
    srcs = ["fastq-parser.cc"],
    hdrs = ["fastq-parser.h"],
    deps = [
        ":fastq",
        "//bio/common:line-parser-base",
        "//bio/common:strings",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@gxl//gxl/file",
        "@gxl//gxl/status:status_macros",
    ],
)

cc_test(
    name = "fastq-parser_test",
    srcs = ["fastq-parser_test.cc"],
    data = ["//bio/fastq/testdata"],
    deps = [
        ":fastq",
        ":fastq-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "fastq-writer",
    hdrs = ["fastq-writer.h"],
    deps = [
        ":fastq",
        "//bio/common:entry-writer-base",
        "@abseil-cpp//absl/base:nullability",
        "@gxl//gxl/file",
    ],
)

cc_test(
    name = "fastq-writer_test",
    srcs = ["fastq-writer_test.cc"],
    deps = [
        ":fastq",
        ":fastq-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
        "@gxl//gxl/file:path",
    ],
)
