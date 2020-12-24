load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "critical_values_calculation_cc_proto",
    deps = [":critical_values_calculation_proto"],
)

proto_library(
    name = "critical_values_calculation_proto",
    srcs = [":critical_values_calculation.proto"],
    deps = [],
)
