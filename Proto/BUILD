load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "wallet_cc_proto",
    deps = [":wallet_proto"],
)

proto_library(
    name = "wallet_proto",
    srcs = [":wallet.proto"],
    deps = [
    "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "coin_cc_proto",
    deps = [":coin_proto"],
)

proto_library(
    name = "coin_proto",
    srcs = [":coin.proto"],
    deps = [
    ],
)
