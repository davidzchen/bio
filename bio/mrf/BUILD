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
