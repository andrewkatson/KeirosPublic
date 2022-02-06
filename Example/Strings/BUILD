package(default_visibility = ["//visibility:public"])

cc_library(
    name = "convert_lib",
    srcs = [
        "convert.cc",
    ],
    hdrs = [":convert.h"],
    deps = ["//Tools/JNI:copy_jni_hdr_lib"],
    alwayslink = True,
)

cc_binary(
    name = "convert",
    linkshared = True,
    deps = [":convert_lib"],
)
