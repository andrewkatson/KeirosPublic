load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "denarii_client",
    srcs = ["denarii_client.cc"],
    hdrs = ["denarii_client.h"],
    deps = [
        "//Client/RandomX:randomx_client",
        "//Proto:wallet_cc_proto",
        "//ProtoBuf/TextProto:read_text_proto",
        "//Tools/General/Strings:hex",
        "//Tools/General/Strings:replace",
        "//Tools/JNI:copy_jni_hdr_lib",
        "@curl",
        "@json",
    ],
    alwayslink = True,
)

cc_binary(
    name = "denarii_client_so",
    linkshared = True,
    deps = [":denarii_client"],
)

py_binary(
    name = "denarii_client_py",
    srcs = ["denarii_client.py"],
    data = ["//Client/RandomX:randomx_client_so"],
    imports = [],
    main = "denarii_client.py",
    deps = ["//:workspace_path_finder"],
)
