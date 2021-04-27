load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])


js_proto_library(
    name = "event_js_proto",
    protos = ["event_proto"]
)

py_proto_library(
    name = "event_py_proto",
    deps = ["event_proto"]
)

java_proto_library(
    name = "event_java_proto",
    deps = [":event_proto"],
)

cc_proto_library(
    name = "event_cc_proto",
    deps = [":event_proto"],
)

proto_library(
    name = "event_proto",
    srcs = [
        "event.proto",
    ],
    deps = [
        # Well known protos should be included as deps in the
        # proto_library rules of the source files importing them.
        # A list of all @com_google_protobuf well known protos can
        # seen with:
        # `bazel query 'kind(proto_library, @com_google_protobuf//:all)'`
        "@com_google_protobuf//:timestamp_proto",
        "//Network/AuthSystem/Authentication/Proto/Token/SessionToken/Base:session_token_proto",
        "//Reactor/Proto/Event/Routing:routing_settings_proto",
        "//Reactor/Proto/Event/Thread:message_thread_info_proto",
        "//Security/Encryption/Proto:signature_info_proto",
        "//Security/Proto:identifier_proto",
    ],
)
