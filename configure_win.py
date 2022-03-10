# Configuration script to set up the dependencies for KeirosPublic automatically with Windows specific configuration

import os

import argparse
import os
import sys
import platform
import urllib.request
import workspace_path_finder
import zipfile

from pathlib import Path

workspace_path = Path()
home = Path()


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
    global workspace_path
    workspace_path = workspace_path_finder.find_workspace_path()

    external_dir_path = workspace_path / "external"

    randomx_win(external_dir_path)

    curl(external_dir_path)


main()
