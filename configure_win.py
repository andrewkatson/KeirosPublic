# Configuration script to set up the dependencies for KeirosPublic automatically with Windows specific configuration

import os

import argparse
import os
import sys
import platform
import urllib.request
import zipfile

from pathlib import Path

parser = argparse.ArgumentParser(description="Process command line flags")
parser.add_argument('--workspace_path', type=str, help='The path to the relevant WORKSPACE file', default='')

args = parser.parse_args()

workspace_path = Path()
home = Path()


def find_workspace_path():
    global workspace_path
    global home

    if args.workspace_path == '':
        # Need to explicitly set this or pass it in as a variable.
        # Andrew's Linux Path
        linux_workspace_path = Path("/home/andrew/KeirosPublic")
        windows_workspace_path = Path("C:/Users/katso/Documents/Github/KeirosPublic")
        # A workspace path that works if not sudo on EC2
        try:
            possible_workspace_path = Path(os.environ["HOME"] + "/KeirosPublic")
            if os.path.exists(possible_workspace_path):
                workspace_path = possible_workspace_path
        except Exception as e:
            print(e)
            print("The HOME variable does not point to the directory")

        # A workspace path that works in sudo on EC2
        try:
            possible_workspace_path = Path("/home/" + os.environ["SUDO_USER"] + "/KeirosPublic")

            if os.path.exists(possible_workspace_path):
                workspace_path = possible_workspace_path
        except Exception as e:
            print(e)
            print("Not on an EC2 using sudo")

        if os.path.exists(linux_workspace_path):
            workspace_path = linux_workspace_path
        elif os.path.exists(windows_workspace_path):
            workspace_path = windows_workspace_path
    else:
        workspace_path = Path(args.workspace_path)

find_workspace_path()

