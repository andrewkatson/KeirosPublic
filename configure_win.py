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


def chdir(path):
    if not os.path.exists(path):
        os.makedirs(path)

    os.chdir(path)


def randomx_win(external_dir_path):
    raw_path = str(external_dir_path)

    chdir(raw_path)

    clone_command = "git clone https://github.com/tevador/RandomX.git"
    os.system(clone_command)

    randomx_path = raw_path + "/RandomX"

    chdir(randomx_path)

    command = "mkdir build && cd build && cmake -DARCH=native -G \"MinGW Makefiles\" .. && mingw32-make"
    os.system(command)

    chdir(external_dir_path)

    mv_command = "move RandomX randomx"
    os.system(mv_command)


def curl(external_dir_path):
    raw_path = str(external_dir_path) + "/curl"

    chdir(raw_path)

    clone_command = "git clone https://github.com/curl/curl.git"
    os.system(clone_command)

    curl_path = raw_path + "/curl"

    chdir(curl_path)

    command = "buildconf && mingw32-make mingw32"
    os.system(command)


def main():
    find_workspace_path()

    external_dir_path = workspace_path / "external"

    randomx_win(external_dir_path)

    curl(external_dir_path)


main()
