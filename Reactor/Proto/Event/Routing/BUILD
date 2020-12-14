load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "routing_settings_cc_proto",
    deps = [":routing_settings_proto"],
)

proto_library(
    name = "routing_settings_proto",
    srcs = [":routing_settings.proto"],
    deps = ["//Network/Proto:broadcast_range_proto"],
)

cc_proto_library(
    name = "routing_failure_cc_proto",
    deps = [":routing_failure_proto"],
)

proto_library(
    name = "routing_failure_proto",
    srcs = [":routing_failure.proto"],
    deps = [
        "//Reactor/Proto/Event/Base:event_proto",
        "//Security/Proto:identifier_proto",
    ],
)
