# A file that moves all the generated python protobufs to %HOME%/py_proto or
# %HOMEDRIVE%%HOMEPATH%\py_proto
# so that they can be readily imported. This assumes build_check was run.
# Needs to be run like ./bazel-bin/move_py_proto after its built.

import pathlib
import os
import re
import shutil
import workspace_path_finder

# If this word is found in the path don't move the proto. It belongs to another directory.
paths_to_exclude = ["external"]

# The WORKSPACE location. We can split any paths along this so that we can generate
# the build path.
workspace_folder = "KeirosPublic"

# A regular expression that will match against text between quotations.
regular_expression_matching_name = '"([^"]*)"'

print("Beginning to move all rules\n\n\n")

for path in pathlib.Path('.').rglob('**/*BUILD'):
    full_path = str(path.resolve())

    # We split along the workspace folder name.
    # should have something like /Folder/To/The/BUILD
    full_path_to_build_file = full_path.split(workspace_folder)[1]

    # Remove the first '/' because we do not need that in a command
    # should have something like Folder/To/The/BUILD
    full_path_without_slash = full_path_to_build_file[1:]

    # Remove the '/BUILD' file at the end of the path
    base_command_path = full_path_without_slash.split('/BUILD')[0]

    # In case this is the top level BUILD File it needs to be handled
    base_command_path = "" if base_command_path == "BUILD:" else base_command_path

    # Get every single "name" field per build file that correspond to py_protos
    names = []
    for line in path.open():
        if 'name = ' in line and "py_proto" in line and "move_py_proto" not in line:
            # It comes in a list of one so just grab the first item.
            build_rule_name = re.findall(regular_expression_matching_name, line)[0]
            names.append(build_rule_name)

    if len(names) > 0:
        workspace_path = workspace_path_finder.find_workspace_path()
        root_path = workspace_path_finder.get_home()
        for name in names:

            # Remove the py_proto and add a _pb2.py to the end of the name
            edited_name = name.split('py_proto')[0] + "pb2.py"

            path_to_proto = f"{workspace_path}/bazel-bin/{base_command_path}/{edited_name}"

            py_proto_path = f"{root_path}/py_proto/{base_command_path}"
            if not os.path.exists(py_proto_path):
                os.makedirs(py_proto_path)

            full_py_proto_path = f"{py_proto_path}/{edited_name}"

            if os.path.exists(full_py_proto_path):
                os.remove(full_py_proto_path)

            shutil.copyfile(path_to_proto, full_py_proto_path)
            