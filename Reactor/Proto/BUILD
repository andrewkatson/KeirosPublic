load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")
package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "critical_values_info_request_js_proto",
    protos = ["critical_values_info_request_proto"]
)

py_proto_library(
    name = "critical_values_info_request_py_proto",
    deps = ["critical_values_info_request_proto"]
)

java_proto_library(
    name = "critical_values_info_request_java_proto",
    deps = [":critical_values_info_request_proto"],
)

cc_proto_library(
    name = "critical_values_info_request_cc_proto",
    deps = [":critical_values_info_request_proto"],
)

proto_library(
    name = "critical_values_info_request_proto",
    srcs = [":critical_values_info_request.proto"],
    deps = [
    	"@com_google_protobuf//:field_mask_proto",
    ],
)


js_proto_library(
    name = "critical_values_info_response_js_proto",
    protos = ["critical_values_info_response_proto"]
)

py_proto_library(
    name = "critical_values_info_response_py_proto",
    deps = ["critical_values_info_response_proto"]
)

java_proto_library(
    name = "critical_values_info_response_java_proto",
    deps = [":critical_values_info_response_proto"],
)

cc_proto_library(
    name = "critical_values_info_response_cc_proto",
    deps = [":critical_values_info_response_proto"],
)

proto_library(
    name = "critical_values_info_response_proto",
    srcs = [":critical_values_info_response.proto"],
    deps = [
    	"//Reactor/CriticalValuesCalculator/Proto:critical_values_calculation_proto",
    ],
)
