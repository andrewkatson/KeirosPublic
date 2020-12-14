load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])


cc_proto_library(
    name = "message_thread_info_cc_proto",
    deps = [":message_thread_info_proto"],
)

proto_library(
    name = "message_thread_info_proto",
    srcs = ["message_thread_info.proto"],
    deps = [
        "//Security/Proto:identifier_proto",
    ],
)