load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")

package(default_visibility = ["//visibility:public"])

js_proto_library(
    name = "new_account_request_js_proto",
    protos = ["new_account_request_proto"],
)

py_proto_library(
    name = "new_account_request_py_proto",
    deps = ["new_account_request_proto"],
)

java_proto_library(
    name = "new_account_request_java_proto",
    deps = [":new_account_request_proto"],
)

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

js_proto_library(
    name = "new_account_response_js_proto",
    protos = ["new_account_response_proto"],
)

py_proto_library(
    name = "new_account_response_py_proto",
    deps = ["new_account_response_proto"],
)

java_proto_library(
    name = "new_account_response_java_proto",
    deps = [":new_account_response_proto"],
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

js_proto_library(
    name = "currency_managemeent_user_info_request_js_proto",
    protos = ["currency_management_user_info_request_proto"],
)

py_proto_library(
    name = "currency_management_user_info_request_py_proto",
    deps = ["currency_management_user_info_request_proto"],
)

java_proto_library(
    name = "currency_management_user_info_request_java_proto",
    deps = ["currency_management_user_info_request_proto"],
)

cc_proto_library(
    name = "currency_management_user_info_request_cc_proto",
    deps = [":currency_management_user_info_request_proto"],
)

proto_library(
    name = "currency_management_user_info_request_proto",
    srcs = [":currency_management_user_info_request.proto"],
    deps = [
        "//Security/Proto:identifier_proto",
        "@com_google_protobuf//:field_mask_proto",
    ],
)

js_proto_library(
    name = "currency_management_user_info_response_js_proto",
    protos = ["currency_management_user_info_response_proto"],
)

py_proto_library(
    name = "currency_management_user_info_response_py_proto",
    deps = ["currency_management_user_info_response_proto"],
)

java_proto_library(
    name = "currency_management_user_info_response_java_proto",
    deps = [":currency_management_user_info_response_proto"],
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

js_proto_library(
    name = "wallet_info_request_js_proto",
    deps = [":wallet_info_request_proto"],
)

py_proto_library(
    name = "wallet_info_request_py_proto",
    deps = [":wallet_info_request_proto"],
)

java_proto_library(
    name = "wallet_info_request_java_proto",
    deps = [":wallet_info_request_proto"],
)

cc_proto_library(
    name = "wallet_info_request_cc_proto",
    deps = [":wallet_info_request_proto"],
)

proto_library(
    name = "wallet_info_request_proto",
    srcs = [":wallet_info_request.proto"],
    deps = [
        "//Security/Proto:identifier_proto",
    ],
)

js_proto_library(
    name = "wallet_info_response_js_proto",
    deps = [":wallet_info_response_proto"],
)

py_proto_library(
    name = "wallet_info_response_py_proto",
    deps = [":wallet_info_response_proto"],
)

java_proto_library(
    name = "wallet_info_response_java_proto",
    deps = [":wallet_info_response_proto"],
)

cc_proto_library(
    name = "wallet_info_response_cc_proto",
    deps = [":wallet_info_response_proto"],
)

proto_library(
    name = "wallet_info_response_proto",
    srcs = [":wallet_info_response.proto"],
    deps = [
        "//Proto:wallet_proto",
    ],
)

js_proto_library(
    name = "currency_management_config_request_js_proto",
    deps = [":currency_management_config_request_proto"],
)

py_proto_library(
    name = "currency_management_config_request_py_proto",
    deps = [":currency_management_config_request_proto"],
)

java_proto_library(
    name = "currency_management_config_request_java_proto",
    deps = [":currency_management_config_request_proto"],
)

cc_proto_library(
    name = "currency_management_config_request_cc_proto",
    deps = [":currency_management_config_request_proto"],
)

proto_library(
    name = "currency_management_config_request_proto",
    srcs = [":currency_management_config_request.proto"],
    deps = [
        "//Proto:wallet_proto",
        "//Security/Proto:identifier_proto",
    ],
)

js_proto_library(
    name = "currency_management_config_response_js_proto",
    deps = [":currency_management_config_response_proto"],
)

py_proto_library(
    name = "currency_management_config_response_py_proto",
    deps = [":currency_management_config_response_proto"],
)

java_proto_library(
    name = "currency_management_config_response_java_proto",
    deps = [":currency_management_config_response_proto"],
)

cc_proto_library(
    name = "currency_management_config_response_cc_proto",
    deps = [":currency_management_config_response_proto"],
)

proto_library(
    name = "currency_management_config_response_proto",
    srcs = [":currency_management_config_response.proto"],
    deps = [
    ],
)

js_proto_library(
    name = "transaction_js_proto",
    deps = [":transaction_proto"],
)

py_proto_library(
    name = "transaction_py_proto",
    deps = [":transaction_proto"],
)

java_proto_library(
    name = "transaction_java_proto",
    deps = [":transaction_proto"],
)

cc_proto_library(
    name = "transaction_cc_proto",
    deps = [":transaction_proto"],
)

proto_library(
    name = "transaction_proto",
    srcs = [":transaction.proto"],
    deps = [
        "//Proto:wallet_proto",
        "//Security/Encryption/Proto:signature_info_proto",
        "//Security/Proto:identifier_proto",
    ],
)
