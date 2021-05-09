load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

js_proto_library(
    name = "critical_values_calculation_js_proto",
    protos = ["critical_values_calculation_proto"]
)

py_proto_library(
    name = "critical_values_calculation_py_proto",
    deps = ["critical_values_calculation_proto"]
)

java_proto_library(
    name = "critical_values_calculation_java_proto",
    deps = [":critical_values_calculation_proto"],
)

cc_proto_library(
    name = "critical_values_calculation_cc_proto",
    deps = [":critical_values_calculation_proto"],
)

proto_library(
    name = "critical_values_calculation_proto",
    srcs = [":critical_values_calculation.proto"],
    deps = [],
)
