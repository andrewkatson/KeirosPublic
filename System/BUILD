load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "execute",
    hdrs = ["execute.h"],
    deps = [
    ],
)

cc_library(
    name = "get_pid",
    hdrs = ["get_pid.h"],
    deps = [
        ":execute",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "get_pid_test",
    srcs = ["get_pid_test.cc"],
    deps = [
        ":get_pid",
        "@gtest",
    ],
)
