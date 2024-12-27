package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sequence",
    hdrs = ["sequence.h"],
)

cc_library(
    name = "line-parser-base",
    hdrs = ["line-parser-base.h"],
    deps = [
        "@abseil-cpp//absl/base:nullability",
        "@abxl//abxl/file",
        "@abxl//abxl/file:filelineiter",
    ],
)

cc_test(
    name = "line-parser-base_test",
    srcs = ["line-parser-base_test.cc"],
    data = ["//bio/common/testdata"],
    deps = [
        ":line-parser-base",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abxl//abxl/file",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
