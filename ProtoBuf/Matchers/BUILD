load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "equals_proto",
    hdrs = [":equals_proto.h"],
    deps = [
        "//ProtoBuf/Timestamp/Proto:timestamp_cc_proto",
        "@com_google_protobuf//:protobuf_headers",
        "@gtest",
        "@keiros_public//Reactor/Proto/Event/Base:event_cc_proto",
    ],
)

cc_test(
    name = "equals_proto_test",
    srcs = ["equals_proto_test.cc"],
    deps = [
        ":equals_proto",
        "//ProtoBuf/Timestamp/Proto:timestamp_cc_proto",
        "@gtest",
        "@keiros_public//Reactor/Base:event_and_message",
        "@keiros_public//Reactor/Proto/Event/Base:event_cc_proto",
        "@keiros_public//Security/Proto:identifier_cc_proto",
    ],
)

cc_library(
    name = "capture",
    hdrs = [":capture.h"],
    deps = [
        "@gtest",
    ],
)

cc_test(
    name = "capture_test",
    srcs = ["capture_test.cc"],
    deps = [
        ":capture",
        "@gtest",
        "@keiros_public//Reactor/Base:event_and_message",
        "@keiros_public//Reactor/Proto/Event/Base:event_cc_proto",
    ],
)

cc_library(
    name = "print",
    srcs = ["print.h"],
    deps = ["@gtest"],
)
