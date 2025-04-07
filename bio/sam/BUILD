package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sam",
    srcs = ["sam.cc"],
    hdrs = ["sam.h"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
    ],
)

cc_test(
    name = "sam_test",
    srcs = ["sam_test.cc"],
    deps = [
        ":sam",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
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

cc_library(
    name = "sam-writer",
    hdrs = ["sam-writer.h"],
    deps = [
        ":sam",
        "//bio/common:entry-writer-base",
        "@abseil-cpp//absl/base:nullability",
        "@gxl//gxl/file",
    ],
)

cc_test(
    name = "sam-writer_test",
    srcs = ["sam-writer_test.cc"],
    deps = [
        ":sam",
        ":sam-writer",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@gxl//gxl/file",
        "@gxl//gxl/file:path",
    ],
)

cc_library(
    name = "cigar",
    srcs = ["cigar.cc"],
    hdrs = ["cigar.h"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
    ],
)

cc_test(
    name = "cigar_test",
    srcs = ["cigar_test.cc"],
    deps = [
        ":cigar",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "cigar-parser",
    srcs = ["cigar-parser.cc"],
    hdrs = ["cigar-parser.h"],
    deps = [
        ":cigar",
        "//bio/sam/internal:cigar-lexer",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:str_format",
        "@gxl//gxl/status:status_macros",
    ],
)

cc_test(
    name = "cigar-parser_test",
    srcs = ["cigar-parser_test.cc"],
    deps = [
        ":cigar",
        ":cigar-parser",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
