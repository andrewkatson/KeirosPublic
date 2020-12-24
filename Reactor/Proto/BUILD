load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "critical_values_info_request_cc_proto",
    deps = [":critical_values_info_request_proto"],
)

proto_library(
    name = "critical_values_info_request_proto",
    srcs = [":critical_values_info_request.proto"],
    deps = [
    	"@com_google_protobuf//:field_mask_proto",
    ],
)

cc_proto_library(
    name = "critical_values_info_response_cc_proto",
    deps = [":critical_values_info_response_proto"],
)

proto_library(
    name = "critical_values_info_response_proto",
    srcs = [":critical_values_info_response.proto"],
    deps = [
    	"//Reactor/CriticalValuesCalculator/Proto:critical_values_calculation_proto",
    ],
)
