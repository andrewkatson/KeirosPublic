load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "geomap_tile_proto",
    srcs = [":geomap_tile.proto"],
    deps = [
        "//Proto/Geometry:multipolygon_proto",
        "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "geomap_tile_cc_proto",
    deps = [":geomap_tile_proto"],
)

java_proto_library(
    name = "geomap_tile_java_proto",
    deps = [":geomap_tile_proto"],
)

py_proto_library(
    name = "geomap_tile_py_proto",
    deps = [":geomap_tile_proto"],
)

js_proto_library(
    name = "geomap_tile_js_proto",
    deps = [":geomap_tile_proto"],
)

proto_library(
    name = "geomap_metadata_proto",
    srcs = [":geomap_metadata.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "geomap_metadata_cc_proto",
    deps = [":geomap_metadata_proto"],
)

java_proto_library(
    name = "geomap_metadata_java_proto",
    deps = [":geomap_metadata_proto"],
)

py_proto_library(
    name = "geomap_metadata_py_proto",
    deps = [":geomap_metadata_proto"],
)

js_proto_library(
    name = "geomap_metadata_js_proto",
    deps = [":geomap_metadata_proto"],
)
