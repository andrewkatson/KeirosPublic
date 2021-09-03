load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "vec4_proto",
    srcs = [":vec4.proto"],
    deps = [],
)

cc_proto_library(
    name = "vec4_cc_proto",
    deps = [":vec4_proto"],
)

java_proto_library(
    name = "vec4_java_proto",
    deps = [":vec4_proto"],
)

py_proto_library(
    name = "vec4_py_proto",
    deps = [":vec4_proto"],
)

js_proto_library(
    name = "vec4_js_proto",
    deps = [":vec4_proto"],
)

proto_library(
    name = "vec2_proto",
    srcs = [":vec2.proto"],
    deps = [],
)

cc_proto_library(
    name = "vec2_cc_proto",
    deps = [":vec2_proto"],
)

java_proto_library(
    name = "vec2_java_proto",
    deps = [":vec2_proto"],
)

py_proto_library(
    name = "vec2_py_proto",
    deps = [":vec2_proto"],
)

js_proto_library(
    name = "vec2_js_proto",
    deps = [":vec2_proto"],
)

proto_library(
    name = "rotation_proto",
    srcs = [":rotation.proto"],
    deps = [":vec2_proto"],
)

cc_proto_library(
    name = "rotation_cc_proto",
    deps = [":rotation_proto"],
)

java_proto_library(
    name = "rotation_java_proto",
    deps = [":rotation_proto"],
)

py_proto_library(
    name = "rotation_py_proto",
    deps = [":rotation_proto"],
)

js_proto_library(
    name = "rotation_js_proto",
    deps = [":rotation_proto"],
)

proto_library(
    name = "vertex_proto",
    srcs = [":vertex.proto"],
    deps = [
        ":vec2_proto",
        "//Proto/General:color_proto",
    ],
)

cc_proto_library(
    name = "vertex_cc_proto",
    deps = [":vertex_proto"],
)

java_proto_library(
    name = "vertex_java_proto",
    deps = [":vertex_proto"],
)

py_proto_library(
    name = "vertex_py_proto",
    deps = [":vertex_proto"],
)

js_proto_library(
    name = "vertex_js_proto",
    deps = [":vertex_proto"],
)
