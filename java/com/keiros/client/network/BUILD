package(default_visibility = ["//visibility:public"])

java_library(
    name = "networking_client",
    srcs = ["NetworkingClient.java"],
    data = [
        "//Network/Client:connection_configs",
        "//Network/Client:networking_client_so",
    ],
    deps = [
        "//Network/Identity/Proto:ip_java_proto",
        "//Reactor/Proto/Event/Base:event_java_proto",
        "//java/com/keiros/reactor/base:event_and_message",
        "//java/com/keiros/tools/general/strings:hex",
        "@com_google_protobuf//:protobuf_java",
    ],
)
