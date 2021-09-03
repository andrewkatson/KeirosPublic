load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "color_proto",
    srcs = [":color.proto"],
    deps = [],
)

cc_proto_library(
    name = "color_cc_proto",
    deps = [":color_proto"],
)

java_proto_library(
    name = "color_java_proto",
    deps = [":color_proto"],
)

py_proto_library(
    name = "color_py_proto",
    deps = [":color_proto"],
)

js_proto_library(
    name = "color_js_proto",
    deps = [":color_proto"],
)
