load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
	name = "broadcast_range_cc_proto",
	deps = [":broadcast_range_proto"]
)

proto_library(
	name = "broadcast_range_proto",
	srcs = [":broadcast_range.proto"],
	deps = ["//Security/Proto:identifier_proto"]
)