load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_java//java:defs.bzl", "java_proto_library")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto_grpc//csharp:defs.bzl", "csharp_proto_library")
load("@rules_proto_grpc//android:defs.bzl", "android_proto_library")
load("@rules_proto_grpc//d:defs.bzl", "d_proto_library")
load("@rules_proto_grpc//c:defs.bzl", "c_proto_library")
load("@rules_proto_grpc//js:defs.bzl", "js_proto_library")
load("@rules_proto_grpc//objc:defs.bzl", "objc_proto_library")
load("@rules_proto_grpc//rust:defs.bzl", "rust_proto_library")
load("@rules_proto_grpc//scala:defs.bzl", "scala_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "authentication_type_proto",
    srcs = ["authentication_type.proto"],
    deps = [
        "//Network/AuthSystem/Authentication/Methods/AuthenticatorApp/Proto:authenticator_app_type_proto",
        "//Network/AuthSystem/Authentication/Methods/PushNotification/Proto:push_notification_type_proto",
    ],
)

cc_proto_library(
    name = "authentication_type_cc_proto",
    deps = [":authentication_type_proto"],
)

java_proto_library(
    name = "authentication_type_java_proto",
    deps = [":authentication_type_proto"],
)


js_proto_library(
    name = "authentication_type_js_proto",
    protos = ["authentication_type_proto"]
)

py_proto_library(
    name = "authentication_type_py_proto",
    deps = [
        ":authentication_type_proto",
    ],
)