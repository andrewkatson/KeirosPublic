load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
	name = "session_token_proto",
	srcs = ["session_token.proto"],
  deps = ["//Security/Proto:identifier_proto",]
)

cc_proto_library(
	name = "session_token_cc_proto",
	deps = [":session_token_proto"],
)