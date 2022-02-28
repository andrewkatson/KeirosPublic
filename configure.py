# Configuration script to set up the dependencies for KeirosPublic automatically.

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


def wolfssl():
    wolfssl_path = workspace_path + "/external/wolfssl"
    os.chdir(wolfssl_path)

    clone_command = "git clone https://github.com/andrewkatson/wolfssl.git"
    os.system(clone_command)

    inside_folder_path = wolfssl_path + "/wolfssl"
    os.chdir(inside_folder_path)

    build_command = "./configure --enable-ipv6 --enable-blake2 --enable-blake2s --enable-keygen --enable-certgen --enable-certreq --enable-ed25519 --enable-eccencrypt && make && sudo make install && mkdir build && cp /usr/local/lib/libwolfssl* " + inside_folder_path + "/build"
    os.system(build_command)


def chdir(path):
    if not os.path.exists(path):
        os.mkdir(path)

    os.chdir(path)


def wolfssl_win():
    wolfssl_path = workspace_path / "external" / "wolfssl"
    chdir(wolfssl_path)

    clone_command = "git clone https://github.com/wolfSSL/wolfssl.git"
    os.system(clone_command)

    make_command = "./autogen.sh && ./configure && make"
    os.system(make_command)


def update_java_path():
    global workspace_path
    workspace_path = workspace_path / "bazel-bin/Network/Client"
    os.environ["LD_LIBRARY_PATH"] = os.environ["LD_LIBRARY_PATH"] + ":" + workspace_path.__str__()


def main():
    find_workspace_path()
    update_java_path()
    if sys.platform == "linux":
        chdir(workspace_path)
        wolfssl()
    elif sys.platform == "msys":
        chdir(workspace_path)
        wolfssl_win()


main()
