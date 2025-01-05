package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bed",
    srcs = ["bed.cc"],
    hdrs = ["bed.h"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "bed_test",
    srcs = ["bed_test.cc"],
    deps = [
        ":bed",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bed-parser",
    srcs = ["bed-parser.cc"],
    hdrs = ["bed-parser.h"],
    deps = [
        ":bed",
        "//bio/common:line-parser-base",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@abxl//abxl/file",
        "@abxl//abxl/status:status_macros",
    ],
)

cc_test(
    name = "bed-parser_test",
    srcs = ["bed-parser_test.cc"],
    data = ["//bio/bed/testdata/bed"],
    deps = [
        ":bed",
        ":bed-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bed-writer",
    srcs = ["bed-writer.cc"],
    hdrs = ["bed-writer.h"],
    deps = [
        ":bed",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@abxl//abxl/file",
        "@abxl//abxl/status:status_macros",
    ],
)

cc_test(
    name = "bed-writer_test",
    srcs = ["bed-writer_test.cc"],
    deps = [
        ":bed",
        ":bed-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@abxl//abxl/file",
        "@abxl//abxl/file:path",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

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
        "@abxl//abxl/file",
        "@abxl//abxl/status:status_macros",
    ],
)

cc_test(
    name = "bedgraph-parser_test",
    srcs = ["bedgraph-parser_test.cc"],
    data = ["//bio/bed/testdata/bedgraph"],
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
        "@abxl//abxl/file",
    ],
)

cc_test(
    name = "bedgraph-writer_test",
    srcs = ["bedgraph-writer_test.cc"],
    deps = [
        ":bedgraph",
        ":bedgraph-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@abxl//abxl/file",
        "@abxl//abxl/file:path",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
