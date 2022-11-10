workspace(name = "keiros_public")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:maven_rules.bzl", "maven_jar")

# the base google protocol buffer code.
http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-21.9",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v21.9.zip"],
)

# gRPC code -- has py_proto_library rule which is useful.
http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "b0d3b876d85e4e4375aa211a52a33b7e8ca9f9d6d97a60c3c844070a700f0ea3",
    strip_prefix = "grpc-1.28.1",
    urls = ["https://github.com/grpc/grpc/archive/v1.28.1.zip"],
)

# proto libraries for grpc. this gives us all the esoteric languages that can be used
http_archive(
    name = "rules_proto_grpc",
    sha256 = "bbe4db93499f5c9414926e46f9e35016999a4e9f6e3522482d3760dc61011070",
    strip_prefix = "rules_proto_grpc-4.2.0",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/4.2.0.tar.gz"],
)

# Google Test
http_archive(
    name = "gtest",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
)

# abseil-cpp
http_archive(
    name = "com_google_absl",
    sha256 = "8400c511d64eb4d26f92c5ec72535ebd0f843067515244e8b50817b0786427f9",
    strip_prefix = "abseil-cpp-c512f118dde6ffd51cb7d8ac8804bbaf4d266c3a",
    urls = ["https://github.com/abseil/abseil-cpp/archive/c512f118dde6ffd51cb7d8ac8804bbaf4d266c3a.zip"],
)

#gson
maven_jar(
    name = "gson",
    artifact = "com.google.code.gson:gson:2.9.0",
    sha1 = "8a1167e089096758b49f9b34066ef98b2f4b37aa",
)

# boost
git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

# gflags
git_repository(
    name = "com_github_gflags_gflags",
    commit = "e171aa2d15ed9eb17054558e0b3a6a413bb01067",
    remote = "https://github.com/gflags/gflags.git",
    shallow_since = "1541971260 +0000",
)

# upb
git_repository(
    name = "upb",
    commit = "d16bf99ac4658793748cda3251226059892b3b7b",
    remote = "https://github.com/protocolbuffers/upb.git",
    shallow_since = "1558031931 -0700"
)

#wolfssl
new_local_repository(
    name = "wolfssl",
    build_file = "external/BUILD.wolfssl",
    path = "external/wolfssl/wolfssl",
)

#json
new_local_repository(
    name = "json",
    build_file = "external/BUILD.json",
    path = "external/json/json",
)

#curl
new_local_repository(
    name = "curl",
    build_file = "external/BUILD.curl",
    path = "external/curl/curl",
)

#randomx
new_local_repository(
    name = "randomx",
    build_file = "external/BUILD.randomx",
    path = "external/randomx",
)

#bigint
new_local_repository(
    name = "bigint",
    build_file = "external/BUILD.bigint",
    path = "external/bigint",
)

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

rules_cc_dependencies()

load("@rules_java//java:repositories.bzl", "rules_java_dependencies", "rules_java_toolchains")

rules_java_dependencies()

rules_java_toolchains()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

load("@rules_python//python:repositories.bzl", "py_repositories")

py_repositories()

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_repos", "rules_proto_grpc_toolchains")
load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_toolchains")

rules_proto_grpc_toolchains()

rules_proto_grpc_repos()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

load("@rules_proto_grpc//csharp:repositories.bzl", rules_proto_grpc_csharp_repos = "csharp_repos")

rules_proto_grpc_csharp_repos()

load("@io_bazel_rules_dotnet//dotnet:deps.bzl", "dotnet_repositories")

dotnet_repositories()

load(
    "@io_bazel_rules_dotnet//dotnet:defs.bzl",
    "dotnet_register_toolchains",
    "dotnet_repositories_nugets",
)

dotnet_register_toolchains()

dotnet_repositories_nugets()

load("@rules_proto_grpc//csharp/nuget:nuget.bzl", "nuget_rules_proto_grpc_packages")

nuget_rules_proto_grpc_packages()

# The set of dependencies loaded here is excessive for android proto alone
# (but simplifies our setup)
load("@rules_proto_grpc//android:repositories.bzl", rules_proto_grpc_android_repos = "android_repos")

rules_proto_grpc_android_repos()

load("@rules_jvm_external//:defs.bzl", "maven_install")
load("@io_grpc_grpc_java//:repositories.bzl", "IO_GRPC_GRPC_JAVA_ARTIFACTS", "IO_GRPC_GRPC_JAVA_OVERRIDE_TARGETS", "grpc_java_repositories")

maven_install(
    artifacts = IO_GRPC_GRPC_JAVA_ARTIFACTS,
    generate_compat_repositories = True,
    override_targets = IO_GRPC_GRPC_JAVA_OVERRIDE_TARGETS,
    repositories = [
        "https://repo.maven.apache.org/maven2/",
    ],
)

load("@maven//:compat.bzl", "compat_repositories")

compat_repositories()

grpc_java_repositories()

load("@build_bazel_rules_android//android:sdk_repository.bzl", "android_sdk_repository")

android_sdk_repository(name = "androidsdk")

load("@rules_proto_grpc//d:repositories.bzl", rules_proto_grpc_d_repos = "d_repos")

rules_proto_grpc_d_repos()

load("@io_bazel_rules_d//d:d.bzl", "d_repositories")

d_repositories()

load("@rules_proto_grpc//c:repositories.bzl", rules_proto_grpc_c_repos = "c_repos")

rules_proto_grpc_c_repos()

load("@upb//bazel:workspace_deps.bzl", "upb_deps")

upb_deps()

load("@rules_proto_grpc//js:repositories.bzl", rules_proto_grpc_js_repos = "js_repos")

rules_proto_grpc_js_repos()

load("@build_bazel_rules_nodejs//:index.bzl", "yarn_install")

yarn_install(
    name = "npm",
    package_json = "@rules_proto_grpc//js:requirements/package.json",  # This should be changed to your local package.json which should contain the dependencies required
    yarn_lock = "@rules_proto_grpc//js:requirements/yarn.lock",
)

load("@rules_proto_grpc//objc:repositories.bzl", rules_proto_grpc_objc_repos = "objc_repos")

rules_proto_grpc_objc_repos()

load("@rules_proto_grpc//rust:repositories.bzl", rules_proto_grpc_rust_repos = "rust_repos")

rules_proto_grpc_rust_repos()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@rules_rust//rust:repositories.bzl", "rust_repositories")

rust_repositories()

load("@rules_proto_grpc//scala:repositories.bzl", rules_proto_grpc_scala_repos = "scala_repos")

rules_proto_grpc_scala_repos()

load("@io_bazel_rules_scala//:scala_config.bzl", "scala_config")

scala_config()

load("@io_bazel_rules_scala//scala:scala.bzl", "scala_repositories")

scala_repositories()

load("@io_bazel_rules_scala//scala_proto:scala_proto.bzl", "scala_proto_repositories")

scala_proto_repositories()

load("@io_bazel_rules_scala//scala:toolchains.bzl", "scala_register_toolchains")

scala_register_toolchains()
