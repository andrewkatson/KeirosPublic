load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "polygon_proto",
    srcs = [":polygon.proto"],
    deps = ["//Proto/Math:vec2_proto"],
)

cc_proto_library(
    name = "polygon_cc_proto",
    deps = [":polygon_proto"],
)

java_proto_library(
    name = "polygon_java_proto",
    deps = [":polygon_proto"],
)

py_proto_library(
    name = "polygon_py_proto",
    deps = [":polygon_proto"],
)

js_proto_library(
    name = "polygon_js_proto",
    deps = [":polygon_proto"],
)

proto_library(
    name = "multipolygon_proto",
    srcs = [":multipolygon.proto"],
    deps = [":polygon_proto"],
)

cc_proto_library(
    name = "multipolygon_cc_proto",
    deps = [":multipolygon_proto"],
)

java_proto_library(
    name = "multipolygon_java_proto",
    deps = [":multipolygon_proto"],
)

py_proto_library(
    name = "multipolygon_py_proto",
    deps = [":multipolygon_proto"],
)

js_proto_library(
    name = "multipolygon_js_proto",
    deps = [":multipolygon_proto"],
)
