package(default_visibility = ["//visibility:public"])

cc_library(
    name = "networking_client",
    srcs = ["networking_client.cc"],
    hdrs = ["networking_client.h"],
    deps = [
        "//Network/Identity/Proto:ip_cc_proto",
        "//Network/Protobuf:asio_adapting",
        "//Network/Protobuf:protobuf_helpers",
        "//Reactor/Base:event_and_message",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@wolfssl",
    ],
)

cc_test(
    name = "networking_client_test",
    srcs = ["networking_client_test.cc"],
    deps = [
        ":networking_client",
        "@gtest",
    ],
)
