load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")
package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "wallet_js_proto",
    protos = ["wallet_proto"]
)

py_proto_library(
    name = "wallet_py_proto",
    deps = ["wallet_proto"]
)

java_proto_library(
    name = "wallet_java_proto",
    deps = ["wallet_proto"],
)

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



js_proto_library(
    name = "coin_js_proto",
    protos = ["coin_proto"]
)

py_proto_library(
    name = "coin_py_proto",
    deps = ["coin_proto"]
)

java_proto_library(
    name = "coin_java_proto",
    deps = ["coin_proto"],
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
