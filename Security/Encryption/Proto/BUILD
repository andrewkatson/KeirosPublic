load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")
package(default_visibility = ["//visibility:public"])

js_proto_library(
    name = "encryption_instance_js_proto",
    protos = [":encryption_instance_proto"]
)

py_proto_library(
    name = "encryption_instance_py_proto",
    deps = [":encryption_instance_proto"]
)

java_proto_library(
    name = "encryption_instance_java_proto",
    deps = [":encryption_instance_proto"],
)

cc_proto_library(
    name = "encryption_instance_cc_proto",
    deps = [":encryption_instance_proto"],
)

proto_library(
    name = "encryption_instance_proto",
    srcs = ["encryption_instance.proto"],
    deps = [
        "@keiros_public//Security/Proto:identifier_proto",
    ],
)


js_proto_library(
    name = "asymmetric_encryption_js_proto",
    protos = ["asymmetric_encryption_proto"]
)

py_proto_library(
    name = "asymmetric_encryption_py_proto",
    deps = ["asymmetric_encryption_proto"]
)

java_proto_library(
    name = "asymmetric_encryption_java_proto",
    deps = [":asymmetric_encryption_proto"],
)

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


js_proto_library(
    name = "symmetric_encryption_js_proto",
    protos = ["symmetric_encryption_proto"]
)

py_proto_library(
    name = "symmetric_encryption_py_proto",
    deps = ["symmetric_encryption_proto"]
)

java_proto_library(
    name = "symmetric_encryption_java_proto",
    deps = [":symmetric_encryption_proto"],
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


js_proto_library(
    name = "signature_info_js_proto",
    protos = ["signature_info_proto"]
)

py_proto_library(
    name = "signature_info_py_proto",
    deps = ["signature_info_proto"]
)

java_proto_library(
    name = "signature_info_java_proto",
    deps = [":signature_info_proto"],
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



js_proto_library(
    name = "obfuscation_support_js_proto",
    protos = ["obfuscation_support_proto"]
)

py_proto_library(
    name = "obfuscation_support_py_proto",
    deps = ["obfuscation_support_proto"]
)

java_proto_library(
    name = "obfuscation_support_java_proto",
    deps = [":obfuscation_support_proto"],
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


js_proto_library(
    name = "key_js_proto",
    protos = ["key_proto"]
)

py_proto_library(
    name = "key_py_proto",
    deps = ["key_proto"]
)

java_proto_library(
    name = "key_java_proto",
    deps = [":key_proto"],
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
        ":encryption_instance_proto"
    ],
)
