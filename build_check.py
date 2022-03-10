# Validate that everything builds.

import os
import pathlib
import re
import subprocess
import argparse

parser = argparse.ArgumentParser(description='Process some flags.')
parser.add_argument('is_external', type=bool, default=False,
                    help='Whether this version of the library is external to another project')

args = parser.parse_args()

# Do not try to rebuild this file. Also do not try to build things in subworkspaces
builds_to_exclude = ["build_check", "external/denarii/denarii:test", "external/denarii/denarii:configure"]

paths_to_exclude = ["external"]

# The WORKSPACE location. We can split any paths along this so that we can generate
# the build path.
workspace_folder = "KeirosPublic"

# A regular expression that will match against text between quotations.
regular_expression_matching_name = '"([^"]*)"'

successes = 0
failures = 0
failure_commands = []

print("Beginning to build all rules\n\n\n")


def external_does_not_appear_twice(some_path):
    result = re.findall("external", some_path)
    if len(result) > 1:
        return False
    return True


for path in pathlib.Path('.').rglob('**/*BUILD'):
    full_path = str(path.resolve())

    exit = False
    for other_path in paths_to_exclude:
        if other_path in full_path:
            if other_path == "external" and not args.is_external and external_does_not_appear_twice(full_path):
                exit = True
                break
    if exit:
        continue

    # We split along the workspace folder name.
    # should have something like /Folder/To/The/BUILD
    full_path_to_build_file = full_path.split(workspace_folder)[1]

    # Remove the first '/' because we do not need that in a command
    # should have something like Folder/To/The/BUILD
    full_path_without_slash = full_path_to_build_file[1:]

    # Remove the '/BUILD' file at the end of the path and replace with ':' because
    # we will append the name of each build rule so we can test each one per BUILD file
    # to see if they all work.
    base_command_path = full_path_without_slash.split('/BUILD')[0] + ':'

    # In case this is the top level BUILD File it needs to be handled
    base_command_path = ":" if base_command_path == "BUILD:" else base_command_path

    # Get every single "name" field per build file.
    names = []
    for line in path.open():
        if 'name = ' in line:
            # It comes in a list of one so just grab the first item.
            build_rule_name = re.findall(regular_expression_matching_name, line)[0]
            names.append(build_rule_name)

    # Now try to execute each build rule.
    for name in names:
        if name in builds_to_exclude:
            continue

        full_path_rule = base_command_path + name

        if full_path_rule in builds_to_exclude:
            continue

        build_command = f"bazel build {full_path_rule}"

        outcome = "SUCCESS"

        try:
            output = subprocess.run([build_command], capture_output=True,
                                    shell=True, check=True)
        except Exception as e:
            outcome = f"FAILURE because {e}"

        if outcome == "SUCCESS":
            successes += 1
        else:
            failures += 1
            failure_commands.append(build_command)

        print(f"Command: {build_command} \n ended with {outcome} \n \n")

print(f"There were {successes} successful builds. \n And there were {failures} failed builds")

if failures > 0:
    for build_command in failure_commands:
        print(build_command + "\n")
