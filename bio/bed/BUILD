package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bed",
    hdrs = ["bed.h"],
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
    data = ["//bio/bed/testdata"],
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
