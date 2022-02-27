load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hex",
    srcs = [],
    hdrs = ["hex.h"],
    deps = [],
)

cc_library(
    name = "replace",
    hdrs = [":replace.h"],
    deps = [
    ],
)

cc_library(
    name = "big_int_from_string",
    hdrs = [":big_int_from_string.h"],
    deps = [
        "@bigint",
    ],
)
