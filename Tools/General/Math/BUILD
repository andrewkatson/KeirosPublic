load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bits",
    hdrs = ["bits.h"],
    deps = [
    ],
)

cc_test(
    name = "bits_test",
    srcs = ["bits_test.cc"],
    deps = [
        ":bits",
        "@gtest",
    ],
)

cc_library(
    name = "combination",
    srcs = ["combination.cc"],
    hdrs = ["combination.h"],
    deps = [
    ],
)

cc_test(
    name = "combination_test",
    srcs = ["combination_test.cc"],
    deps = [
        ":combination",
        "@gtest",
    ],
)