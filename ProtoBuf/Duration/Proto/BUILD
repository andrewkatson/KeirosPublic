package(default_visibility = ["//visibility:public"])

# For some reason the duration_cc_proto defined in the BUILD file for protobuf isn't working so
# I just redefine it here
cc_proto_library(
    name = "duration_cc_proto",
    deps = ["@com_google_protobuf//:duration_proto"],
)

java_proto_library(
    name = "duration_java_proto",
    deps = ["@com_google_protobuf//:duration_proto"],
)
