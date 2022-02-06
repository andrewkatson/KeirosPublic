package(default_visibility = ["//visibility:public"])

java_library(
    name = "networking_client",
    srcs = ["NetworkingClient.java"],
    data = ["//Network/Client:networking_client"],
    deps = [
        "//java/com/keiros/reactor/base:event_and_message",
        "//java/com/keiros/tools/general/strings:hex",
        "@com_google_protobuf//:protobuf_java",
        "@keiros_public//Network/Identity/Proto:ip_java_proto",
        "@keiros_public//Reactor/Proto/Event/Base:event_java_proto",
    ],
)
