load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")


package(default_visibility = ["//visibility:public"])

proto_library(
    name = "authenticator_app_type_proto",
    srcs = ["authenticator_app_type.proto"],
)

cc_proto_library(
    name = "authenticator_app_type_cc_proto",
    deps = [":authenticator_app_type_proto"],
)

java_proto_library(
    name = "authenticator_app_type_java_proto",
    deps = [":authenticator_app_type_proto"],
)

js_proto_library(
    name = "authenticator_app_type_js_proto",
    protos = ["authenticator_app_type_proto"]
)

py_proto_library(
    name = "authenticator_app_type_py_proto",
    deps = [
        ":authenticator_app_type_proto",
    ],
)