load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])



js_proto_library(
    name = "mining_forwarding_user_info_request_js_proto",
    protos = ["mining_forwarding_user_info_request_proto"]
)

py_proto_library(
    name = "mining_forwarding_user_info_request_py_proto",
    deps = ["mining_forwarding_user_info_request_proto"]
)

java_proto_library(
    name = "mining_forwarding_user_info_request_java_proto",
    deps = [":mining_forwarding_user_info_request_proto"],
)

cc_proto_library(
    name = "mining_forwarding_user_info_request_cc_proto",
    deps = [":mining_forwarding_user_info_request_proto"],
)

proto_library(
    name = "mining_forwarding_user_info_request_proto",
    srcs = [":mining_forwarding_user_info_request.proto"],
    deps = [
        "@com_google_protobuf//:field_mask_proto",
        "//Security/Proto:identifier_proto",
    ],
)



js_proto_library(
    name = "mining_forwarding_user_info_response_js_proto",
    protos = ["mining_forwarding_user_info_response_proto"]
)

py_proto_library(
    name = "mining_forwarding_user_info_response_py_proto",
    deps = ["mining_forwarding_user_info_response_proto"]
)

java_proto_library(
    name = "mining_forwarding_user_info_response_java_proto",
    deps = [":mining_forwarding_user_info_response_proto"],
)

cc_proto_library(
    name = "mining_forwarding_user_info_response_cc_proto",
    deps = [":mining_forwarding_user_info_response_proto"],
)

proto_library(
    name = "mining_forwarding_user_info_response_proto",
    srcs = [":mining_forwarding_user_info_response.proto"],
    deps = [],
)


js_proto_library(
    name = "new_user_request_js_proto",
    protos = ["new_user_request_proto"]
)

py_proto_library(
    name = "new_user_request_py_proto",
    deps = ["new_user_request_proto"]
)

java_proto_library(
    name = "new_user_request_java_proto",
    deps = [":new_user_request_proto"],
)

cc_proto_library(
    name = "new_user_request_cc_proto",
    deps = [":new_user_request_proto"],
)

proto_library(
    name = "new_user_request_proto",
    srcs = [":new_user_request.proto"],
    deps = [
        "//Proto:coin_proto",
        "//Security/Proto:identifier_proto",
    ],
)


js_proto_library(
    name = "new_user_response_js_proto",
    protos = ["new_user_response_proto"]
)

py_proto_library(
    name = "new_user_response_py_proto",
    deps = ["new_user_response_proto"]
)

java_proto_library(
    name = "new_user_response_java_proto",
    deps = [":new_user_response_proto"],
)

cc_proto_library(
    name = "new_user_response_cc_proto",
    deps = [":new_user_response_proto"],
)

proto_library(
    name = "new_user_response_proto",
    srcs = [":new_user_response.proto"],
    deps = [
        "//Proto:wallet_proto",
        "//Security/Proto:identifier_proto",
    ],
)

