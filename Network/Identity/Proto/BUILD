load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

js_proto_library(
    name = "ip_js_proto",
    deps = [":ip_proto"],
)

py_proto_library(
    name = "ip_py_proto",
    deps = [":ip_proto"],
)

java_proto_library(
    name = "ip_java_proto",
    deps = [":ip_proto"],
)

cc_proto_library(
    name = "ip_cc_proto",
    deps = [":ip_proto"],
)

proto_library(
    name = "ip_proto",
    srcs = [":ip.proto"],
)

js_proto_library(
    name = "connection_config_js_proto",
    deps = [":connection_config_proto"],
)

py_proto_library(
    name = "connection_config_py_proto",
    deps = [":connection_config_proto"],
)

java_proto_library(
    name = "connection_config_java_proto",
    deps = [":connection_config_proto"],
)

cc_proto_library(
    name = "connection_config_cc_proto",
    deps = [":connection_config_proto"],
)

proto_library(
    name = "connection_config_proto",
    srcs = [":connection_config.proto"],
    deps = [":ip_proto"],
)
