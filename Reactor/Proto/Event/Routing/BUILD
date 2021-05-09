load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "routing_settings_js_proto",
    protos = ["routing_settings_proto"]
)

py_proto_library(
    name = "routing_settings_py_proto",
    deps = ["routing_settings_proto"]
)

java_proto_library(
    name = "routing_settings_java_proto",
    deps = [":routing_settings_proto"],
)

cc_proto_library(
    name = "routing_settings_cc_proto",
    deps = [":routing_settings_proto"],
)

proto_library(
    name = "routing_settings_proto",
    srcs = [":routing_settings.proto"],
    deps = ["//Network/Proto:broadcast_range_proto"],
)


js_proto_library(
    name = "routing_failure_js_proto",
    protos = ["routing_failure_proto"]
)

py_proto_library(
    name = "routing_failure_py_proto",
    deps = ["routing_failure_proto"]
)

java_proto_library(
    name = "routing_failure_java_proto",
    deps = [":routing_failure_proto"],
)

cc_proto_library(
    name = "routing_failure_cc_proto",
    deps = [":routing_failure_proto"],
)

proto_library(
    name = "routing_failure_proto",
    srcs = [":routing_failure.proto"],
    deps = [
        "//Reactor/Proto/Event/Base:event_proto",
        "//Security/Proto:identifier_proto",
    ],
)
