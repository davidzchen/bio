package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bed",
    srcs = ["bed.cc"],
    hdrs = ["bed.h"],
    deps = [
        "//bio/common:sequence",
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "bed_test",
    srcs = ["bed_test.cc"],
    deps = [
        ":bed",
        "//bio/common:sequence",
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
        "//bio/common:sequence",
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
    name = "bed-parser_test",
    srcs = ["bed-parser_test.cc"],
    data = ["//bio/bed/testdata"],
    deps = [
        ":bed",
        ":bed-parser",
        "//bio/common:sequence",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "bed-writer",
    hdrs = ["bed-writer.h"],
    deps = [
        ":bed",
        "//bio/common:entry-writer-base",
        "@abseil-cpp//absl/base:nullability",
        "@gxl//gxl/file",
    ],
)

cc_test(
    name = "bed-writer_test",
    srcs = ["bed-writer_test.cc"],
    deps = [
        ":bed",
        ":bed-writer",
        "//bio/common:sequence",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
        "@gxl//gxl/file:path",
    ],
)
