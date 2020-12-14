load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")

package(default_visibility = ["//visibility:public"])

py_proto_library(
    name = "identifier_py_proto",
    deps = [
        ":identifier_proto",
    ],
)

java_proto_library(
    name = "identifier_java_proto",
    deps = [
        ":identifier_proto",
    ],
)

cc_proto_library(
    name = "identifier_cc_proto",
    deps = [":identifier_proto"],
)

proto_library(
    name = "identifier_proto",
    srcs = ["identifier.proto"],
)
