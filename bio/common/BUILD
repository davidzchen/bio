package(default_visibility = ["//visibility:public"])

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

cc_library(
    name = "strings",
    srcs = ["strings.cc"],
    hdrs = ["strings.h"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "strings_test",
    srcs = ["strings_test.cc"],
    deps = [
        ":strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "entry-writer-base",
    hdrs = ["entry-writer-base.h"],
    deps = [
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
