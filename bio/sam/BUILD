package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sam",
    hdrs = ["sam.h"],
)

cc_library(
    name = "sam-parser",
    srcs = ["sam-parser.cc"],
    hdrs = ["sam-parser.h"],
    deps = [
        ":sam",
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
    name = "sam-parser_test",
    srcs = ["sam-parser_test.cc"],
    data = ["//bio/sam/testdata"],
    deps = [
        ":sam",
        ":sam-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
