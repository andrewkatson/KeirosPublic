load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "denarii_client",
    srcs = ["denarii_client.cc"],
    hdrs = ["denarii_client.h"],
    deps = [
        "//Proto:wallet_cc_proto",
        "//ProtoBuf/TextProto:read_text_proto",
        "//Tools/General/Strings:big_int_from_string",
        "//Tools/General/Strings:hex",
        "//Tools/General/Strings:replace",
        "//Tools/JNI:copy_jni_hdr_lib",
        "@bigint",
        "@curl",
        "@json",
        "@randomx",
    ],
    alwayslink = True
)

cc_binary(
    name = "denarii_client_so",
    linkshared = True,
    deps = [":denarii_client"],
)

