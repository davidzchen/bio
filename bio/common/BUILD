package(default_visibility = ["//visibility:public"])

cc_library(
    name = "line-parser-base",
    hdrs = ["line-parser-base.h"],
    deps = [
        ":sequence",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@gxl//gxl/file",
        "@gxl//gxl/file:filelineiter",
    ],
)

cc_test(
    name = "line-parser-base_test",
    srcs = ["line-parser-base_test.cc"],
    data = ["//bio/common/testdata"],
    deps = [
        ":line-parser-base",
        ":sequence",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
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
        "@gxl//gxl/file",
        "@gxl//gxl/status:status_macros",
    ],
)

cc_library(
    name = "sequence",
    hdrs = ["sequence.h"],
)

cc_library(
    name = "word-iterator",
    srcs = ["word-iterator.cc"],
    hdrs = ["word-iterator.h"],
    deps = [
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "word-iterator_test",
    srcs = ["word-iterator_test.cc"],
    deps = [
        ":word-iterator",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
