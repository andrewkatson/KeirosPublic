package(default_visibility = ["//visibility:public"])

java_test(
    name = "NetworkingClientTest",
    srcs = ["NetworkingClientTest.java"],
    deps = [
        "//java/com/keiros/client/network:networking_client",
        "//java/com/keiros/reactor/base:event_and_message",
        "@com_google_protobuf//:protobuf_java",
        "@keiros_public//Network/Identity/Proto:ip_java_proto",
        "@keiros_public//Reactor/Proto/Event/Base:event_java_proto",
    ],
)
