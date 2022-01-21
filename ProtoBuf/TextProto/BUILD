load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "read_text_proto",
    hdrs = ["read_text_proto.h"],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@gtest",
    ],
)

cc_test(
    name = "read_text_proto_test",
    srcs = ["read_text_proto_test.cc"],
    data = ["//ProtoBuf/TextProto/TestData:test_data"],
    deps = [
        ":read_text_proto",
        "//ProtoBuf/Matchers:equals_proto",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@gtest",
        "@keiros_public//Reactor/Proto/Event/Base:event_cc_proto",
    ],
)

cc_library(
    name = "write_text_proto",
    hdrs = ["write_text_proto.h"],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@gtest",
    ],
)

cc_test(
    name = "write_text_proto_test",
    srcs = ["write_text_proto_test.cc"],
    data = ["//ProtoBuf/TextProto/TestData:test_data"],
    deps = [
        ":read_text_proto",
        ":write_text_proto",
        "//ProtoBuf/Matchers:equals_proto",
        "@com_google_protobuf//:protobuf",
        "@com_google_protobuf//:protobuf_headers",
        "@gtest",
        "@keiros_public//Reactor/Proto/Event/Base:event_cc_proto",
    ],
)
