load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")


package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "identifier_js_proto",
    protos = ["identifier_proto"]
)

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



js_proto_library(
    name = "validity_period_js_proto",
    protos = ["validity_period_proto"]
)

py_proto_library(
    name = "validity_period_py_proto",
    deps = [
        ":validity_period_proto",
    ],
)

java_proto_library(
    name = "validity_period_java_proto",
    deps = [":validity_period_proto"],
)

cc_proto_library(
    name = "validity_period_cc_proto",
    deps = [":validity_period_proto"],
)

proto_library(
    name = "validity_period_proto",
    srcs = ["validity_period.proto"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
    ],
)