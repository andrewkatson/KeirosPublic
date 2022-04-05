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
        "//Tools/General/Strings:hex",
        "//Tools/JNI:copy_jni_hdr_lib",
		"@wolfssl",
    ],
    alwayslink = True,
)

cc_binary(
    name = "networking_client_so",
    linkshared = True,
    deps = [":networking_client"],
)

cc_test(
    name = "networking_client_test",
    srcs = ["networking_client_test.cc"],
    data = [":connection_configs"],
    deps = [
        ":networking_client",
        "@gtest",
    ],
)

filegroup(
    name = "connection_configs",
    srcs = [
        "ca.pem",
        "test.key.pem",
        "test-server.pem",
    ],
)
