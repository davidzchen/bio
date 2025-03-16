package(default_visibility = ["//bio/sam:__pkg__"])

cc_library(
    name = "cigar-lexer",
    srcs = ["cigar-lexer.cc"],
    hdrs = ["cigar-lexer.h"],
    deps = [
        "//bio/sam:cigar",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
    ],
)

cc_test(
    name = "cigar-lexer_test",
    srcs = ["cigar-lexer_test.cc"],
    deps = [
        ":cigar-lexer",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/status:statusor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
