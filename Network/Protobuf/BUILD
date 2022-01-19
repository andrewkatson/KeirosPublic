package(default_visibility = ["//visibility:public"])

cc_library(
    name = "protobuf_helpers",
    srcs = ["protobuf_helpers.cpp"],
    hdrs = ["protobuf_helpers.h"],
    deps = ["@com_google_protobuf//:protobuf"],
)

cc_library(
    name = "asio_adapting",
    srcs = [],
    hdrs = ["asio_adapting.h"],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@wolfssl",
    ],
)
