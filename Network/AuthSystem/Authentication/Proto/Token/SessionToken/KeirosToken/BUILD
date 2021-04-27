load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//csharp:defs.bzl", "csharp_proto_library")
load("@rules_proto_grpc//android:defs.bzl", "android_proto_library")
load("@rules_proto_grpc//d:defs.bzl", "d_proto_library")
load("@rules_proto_grpc//c:defs.bzl", "c_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
	name = "keiros_session_token_proto",
	srcs = ["keiros_session_token.proto"],
  deps = [
    "//Network/AuthSystem/Authentication/Proto:authentication_type_proto",
    "//Security/Encryption/Proto:signature_info_proto",
    "//Security/Proto:identifier_proto",
    "//Security/Proto:validity_period_proto",
  ]
)

cc_proto_library(
	name = "keiros_session_token_cc_proto",
	deps = [":keiros_session_token_proto"],
)

js_proto_library(
    name = "keiros_session_token_js_proto",
    protos = ["keiros_session_token_proto"]
)

py_proto_library(
    name = "keiros_session_token_py_proto",
    deps = ["keiros_session_token_proto"]
)

java_proto_library(
    name = "keiros_session_token_java_proto",
    deps = [":keiros_session_token_proto"],
)