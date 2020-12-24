load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "new_account_request_cc_proto",
    deps = [":new_account_request_proto"],
)

proto_library(
    name = "new_account_request_proto",
    srcs = [":new_account_request.proto"],
    deps = [
        "//Proto:coin_proto",
        "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "new_account_response_cc_proto",
    deps = [":new_account_response_proto"],
)

proto_library(
    name = "new_account_response_proto",
    srcs = [":new_account_response.proto"],
    deps = [
        "//Proto:coin_proto",
        "//Proto:wallet_proto",
        "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "currency_management_user_info_request_cc_proto",
    deps = [":currency_management_user_info_request_proto"],
)

proto_library(
    name = "currency_management_user_info_request_proto",
    srcs = [":currency_management_user_info_request.proto"],
    deps = [
        "@com_google_protobuf//:field_mask_proto",
        "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "currency_management_user_info_response_cc_proto",
    deps = [":currency_management_user_info_response_proto"],
)

proto_library(
    name = "currency_management_user_info_response_proto",
    srcs = [":currency_management_user_info_response.proto"],
    deps = [
    ],
)
