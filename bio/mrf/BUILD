package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mrf",
    srcs = ["mrf.cc"],
    hdrs = ["mrf.h"],
    deps = [
        "//bio/common:sequence",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@gxl//gxl/status:status_macros",
    ],
)

cc_test(
    name = "mrf_test",
    srcs = ["mrf_test.cc"],
    deps = [
        ":mrf",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mrf-parser",
    srcs = ["mrf-parser.cc"],
    hdrs = ["mrf-parser.h"],
    deps = [
        ":mrf",
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
    name = "mrf-parser_test",
    srcs = ["mrf-parser_test.cc"],
    data = ["//bio/mrf/testdata"],
    deps = [
        ":mrf",
        ":mrf-parser",
        "//bio/common:sequence",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
