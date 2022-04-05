package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows",
    constraint_values = [
        "@bazel_tools//platforms:windows",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "randomx_client",
    srcs = ["randomx_client.cc"],
    hdrs = ["randomx_client.h"],
    deps = [
        "//Tools/General/Strings:big_int_from_string",
        "@bigint",
    ] + select({
        "windows": [
			"@randomx//:randomx_windows",
        ],
        "//conditions:default": ["@randomx",],
    }),
    alwayslink = True,
)


cc_binary(
    name = "randomx_client_so",
    srcs = ["randomx_client.cc", "randomx_client.h"],
    deps = [
        "//Tools/General/Strings:big_int_from_string",
        "@bigint",
    ]  + select({
        "windows": [
			"@randomx//:randomx_windows",
        ],
        "//conditions:default": ["@randomx",],
    }),
    linkstatic = True,
    linkshared = True,
)