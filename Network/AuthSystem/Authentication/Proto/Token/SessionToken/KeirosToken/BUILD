load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
	name = "keiros_session_token_proto",
	srcs = ["keiros_session_token.proto"],
  deps = [
    "//Network/AuthSystem/Authentication/Proto:authentication_type_proto",
    "//Security/Encryption/Proto:signature_info_proto",
    "//Security/Proto:identifier_proto",
    "//Security/Proto:validity_period_proto",
  ]
)

cc_proto_library(
	name = "keiros_session_token_cc_proto",
	deps = [":keiros_session_token_proto"],
)