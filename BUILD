package(default_visibility = ["//visibility:public"])

py_binary(
    name = "build_check",
    srcs = ["build_check.py"],
    deps = [],
    imports = [],
)

py_binary(
    name = "configure",
    srcs = ["configure.py"],
    deps = [":workspace_path_finder"],
    imports = [":workspace_path_finder"],
)

py_binary(
    name = "configure_win",
    srcs = ["configure_win.py"],
    deps = [":workspace_path_finder"],
    imports = [":workspace_path_finder"],
)

py_binary(
  name = "move_py_proto",
  srcs = ["move_py_proto.py"],
  deps = [":workspace_path_finder"],
  imports = [":workspace_path_finder"],
)

py_binary(
  name = "workspace_path_finder",
  srcs = ["workspace_path_finder.py"],
  deps = [],
  imports = [],
)