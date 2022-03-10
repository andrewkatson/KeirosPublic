# Helps in finding the current workspace path.

import argparse
import os
import pathlib

parser = argparse.ArgumentParser(description="Process command line flags")
parser.add_argument('--workspace_path', type=str, help='The path to the relevant WORKSPACE file', default='')

args = parser.parse_args()


def find_workspace_path():
    workspace_path = pathlib.Path()

    if args.workspace_path == '':
        # Need to explicitly set this or pass it in as a variable.
        linux_workspace_path = pathlib.Path("/home/andrew/KeirosPublic")
        windows_workspace_path = pathlib.Path("C:/Users/katso/Documents/Github/KeirosPublic")

        # A workspace path that works if not sudo on EC2
        try:
            possible_workspace_path = pathlib.Path(os.environ["HOME"] + "/KeirosPublic")
            if os.path.exists(possible_workspace_path):
                workspace_path = possible_workspace_path
        except Exception as e:
            print(e)
            print("The HOME variable does not point to the directory")

        # A workspace path that works in sudo on EC2
        try:
            possible_workspace_path = pathlib.Path("/home/" + os.environ["SUDO_USER"] + "/KeirosPublic")

            if os.path.exists(possible_workspace_path):
                workspace_path = possible_workspace_path
        except Exception as e:
            print(e)
            print("Not on an EC2 using sudo")

        # A workspace path that works on Windows
        try:
            possible_workspace_path = pathlib.Path(
                os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"] + "\\Documents\\Github\\KeirosPublic")

            if os.path.exists(possible_workspace_path):
                workspace_path = possible_workspace_path
        except Exception as e:
            print(e)
            print("Not on Windows")

        if os.path.exists(workspace_path):
            print("Using derived workspace path")
            pass
        elif os.path.exists(linux_workspace_path):
            print("Overriding with predefined linux workspace path")
            workspace_path = linux_workspace_path
        elif os.path.exists(windows_workspace_path):
            print("Overriding with predefined windows workspace path")
            workspace_path = windows_workspace_path
    else:
        workspace_path = pathlib.Path(args.workspace_path)

    return workspace_path


def get_home():
    linux_home = ""
    windows_home = ""

    try:
        linux_home = pathlib.Path(os.environ["HOME"])
    except Exception as e:
        print(e)
    try:
        windows_home = pathlib.Path(os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"])
    except Exception as e:
        print(e)

    if os.path.exists(linux_home):
        return linux_home
    else:
        return windows_home
