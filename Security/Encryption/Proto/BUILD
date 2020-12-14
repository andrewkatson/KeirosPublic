load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "asymmetric_encryption_cc_proto",
    deps = [":asymmetric_encryption_proto"],
)

proto_library(
    name = "asymmetric_encryption_proto",
    srcs = ["asymmetric_encryption.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "symmetric_encryption_cc_proto",
    deps = [":symmetric_encryption_proto"],
)

proto_library(
    name = "symmetric_encryption_proto",
    srcs = ["symmetric_encryption.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "signature_info_cc_proto",
    deps = [":signature_info_proto"],
)

proto_library(
    name = "signature_info_proto",
    srcs = ["signature_info.proto"],
    deps = [
        ":key_proto",
        ":obfuscation_support_proto",
        "//Security/Proto:identifier_proto",
    ],
)

cc_proto_library(
    name = "obfuscation_support_cc_proto",
    deps = [":obfuscation_support_proto"],
)

proto_library(
    name = "obfuscation_support_proto",
    srcs = ["obfuscation_support.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "key_cc_proto",
    deps = [":key_proto"],
)

proto_library(
    name = "key_proto",
    srcs = ["key.proto"],
    deps = [
        ":asymmetric_encryption_proto",
        ":symmetric_encryption_proto",
    ],
)
