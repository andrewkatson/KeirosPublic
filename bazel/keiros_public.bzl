load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:java.bzl", "java_import_external")
load("@bazel_tools//tools/build_defs/repo:maven_rules.bzl", "maven_jar")

def keiros_public_deps():
    if "rules_cc" not in native.existing_rules():
        # C++ protocol buffer rules for Bazel.
        http_archive(
            name = "rules_cc",
            sha256 = "954b7a3efc8752da957ae193a13b9133da227bdacf5ceb111f2e11264f7e8c95",
            strip_prefix = "rules_cc-9e10b8a6db775b1ecd358d8ddd3dab379a2c29a5",
            urls = ["https://github.com/bazelbuild/rules_cc/archive/9e10b8a6db775b1ecd358d8ddd3dab379a2c29a5.zip"],
        )

    if "rules_java" not in native.existing_rules():
        # rules_java defines rules for generating Java code from Protocol Buffers.
        http_archive(
            name = "rules_java",
            sha256 = "ccf00372878d141f7d5568cedc4c42ad4811ba367ea3e26bc7c43445bbc52895",
            strip_prefix = "rules_java-d7bf804c8731edd232cb061cb2a9fe003a85d8ee",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_java/archive/d7bf804c8731edd232cb061cb2a9fe003a85d8ee.tar.gz",
                "https://github.com/bazelbuild/rules_java/archive/d7bf804c8731edd232cb061cb2a9fe003a85d8ee.tar.gz",
            ],
        )

    if "rules_proto" not in native.existing_rules():
        # rules_proto defines abstract rules for building Protocol Buffers.
        http_archive(
            name = "rules_proto",
            sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
            strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
                "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
            ],
        )

    if "rules_python" not in native.existing_rules():
        # language specific rules for building python with Bazel.
        http_archive(
            name = "rules_python",
            sha256 = "e5470e92a18aa51830db99a4d9c492cc613761d5bdb7131c04bd92b9834380f6",
            strip_prefix = "rules_python-4b84ad270387a7c439ebdccfd530e2339601ef27",
            urls = ["https://github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz"],
        )

    if "com_google_protobuf" not in native.existing_rules():
        # the base google protocol buffer code.
        http_archive(
            name = "com_google_protobuf",
            sha256 = "60d2012e3922e429294d3a4ac31f336016514a91e5a63fd33f35743ccfe1bd7d",
            strip_prefix = "protobuf-3.11.0",
            urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.11.0.zip"],
        )

    if "com_github_grpc_grpc" not in native.existing_rules():
        # gRPC code -- has py_proto_library rule which is useful.
        http_archive(
            name = "com_github_grpc_grpc",
            sha256 = "b0d3b876d85e4e4375aa211a52a33b7e8ca9f9d6d97a60c3c844070a700f0ea3",
            strip_prefix = "grpc-1.28.1",
            urls = ["https://github.com/grpc/grpc/archive/v1.28.1.zip"],
        )

    if "rules_proto_grpc" not in native.existing_rules():
        # proto libraries for grpc. this gives us all the esoteric languages that can be used
        http_archive(
            name = "rules_proto_grpc",
            sha256 = "7954abbb6898830cd10ac9714fbcacf092299fda00ed2baf781172f545120419",
            strip_prefix = "rules_proto_grpc-3.1.1",
            urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/3.1.1.tar.gz"],
        )
