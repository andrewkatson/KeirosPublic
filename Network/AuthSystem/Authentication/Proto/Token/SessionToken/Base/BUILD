load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
	name = "session_token_proto",
	srcs = ["session_token.proto"],
  deps = ["//Security/Proto:identifier_proto",]
)

cc_proto_library(
	name = "session_token_cc_proto",
	deps = [":session_token_proto"],
)

js_proto_library(
    name = "session_token_js_proto",
    protos = ["session_token_proto"]
)

py_proto_library(
    name = "session_token_py_proto",
    deps = ["session_token_proto"]
)

java_proto_library(
    name = "session_token_java_proto",
    deps = [":session_token_proto"],
)