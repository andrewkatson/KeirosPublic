package(default_visibility = ["//visibility:public"])

# For some reason the timestamp_cc_proto defined in the BUILD file for protobuf isn't working so
# I just redefine it here
cc_proto_library(
    name = "timestamp_cc_proto",
    deps = ["@com_google_protobuf//:timestamp_proto"],
)

java_proto_library(
    name = "timestamp_java_proto",
    deps = ["@com_google_protobuf//:timestamp_proto"],
)
