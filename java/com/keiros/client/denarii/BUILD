package(default_visibility = ["//visibility:public"])

java_library(
    name = "denarii_client",
    srcs = ["DenariiClient.java"],
    data = [
        "//Client/Denarii:denarii_client_so",
    ],
    deps = [
        "//Proto:wallet_java_proto",
        "//Reactor/Proto/Event/Base:event_java_proto",
        "//java/com/keiros/reactor/base:event_and_message",
        "//java/com/keiros/tools/general/strings:hex",
        "@com_google_protobuf//:protobuf_java",
    ],
)
