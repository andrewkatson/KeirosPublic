load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "broadcast_range_js_proto",
    protos = ["broadcast_range_proto"]
)

py_proto_library(
    name = "broadcast_range_py_proto",
    deps = ["broadcast_range_proto"]
)

java_proto_library(
    name = "broadcast_range_java_proto",
    deps = [":broadcast_range_proto"],
)

cc_proto_library(
	name = "broadcast_range_cc_proto",
	deps = [":broadcast_range_proto"]
)

proto_library(
	name = "broadcast_range_proto",
	srcs = [":broadcast_range.proto"],
	deps = ["//Security/Proto:identifier_proto"]
)