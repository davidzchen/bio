package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bedgraph",
    hdrs = ["bedgraph.h"],
    deps = [
        "@abseil-cpp//absl/strings:str_format",
    ],
)

cc_test(
    name = "bedgraph_test",
    srcs = ["bedgraph_test.cc"],
    deps = [
        ":bedgraph",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bedgraph-parser",
    srcs = ["bedgraph-parser.cc"],
    hdrs = ["bedgraph-parser.h"],
    deps = [
        ":bedgraph",
        "//bio/common:line-parser-base",
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
    name = "bedgraph-parser_test",
    srcs = ["bedgraph-parser_test.cc"],
    data = ["//bio/bedgraph/testdata"],
    deps = [
        ":bedgraph",
        ":bedgraph-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bedgraph-writer",
    hdrs = ["bedgraph-writer.h"],
    deps = [
        ":bedgraph",
        "//bio/common:entry-writer-base",
        "@abseil-cpp//absl/base:nullability",
        "@gxl//gxl/file",
    ],
)

cc_test(
    name = "bedgraph-writer_test",
    srcs = ["bedgraph-writer_test.cc"],
    deps = [
        ":bedgraph",
        ":bedgraph-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
        "@gxl//gxl/file:path",
    ],
)
