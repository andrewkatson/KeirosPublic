# Configuration script to set up the dependencies for KeirosPublic automatically.

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


def wolfssl():
    wolfssl_path = workspace_path / "external/wolfssl"
    chdir(wolfssl_path)

    clone_command = "git clone https://github.com/wolfSSL/wolfssl.git"
    os.system(clone_command)

    inside_folder_path = str(wolfssl_path) + "/wolfssl"
    chdir(inside_folder_path)

    build_command = "./configure --enable-ipv6 --enable-blake2 --enable-blake2s --enable-keygen --enable-certgen --enable-certreq --enable-ed25519 --enable-eccencrypt && make && sudo make install && mkdir build && cp /usr/local/lib/libwolfssl* " + inside_folder_path + "/build"
    os.system(build_command)


def wolfssl_win():
    wolfssl_path = workspace_path / "external/wolfssl"
    chdir(wolfssl_path)

    clone_command = "git clone https://github.com/wolfSSL/wolfssl.git"
    os.system(clone_command)

    inside_folder_path = str(wolfssl_path) + "/wolfssl"
    os.chdir(inside_folder_path)

    make_command = "./autogen.sh &&  ./configure --host=none-none-none && make"
    os.system(make_command)


def curl():
    curl_path = workspace_path / "external/curl"
    chdir(curl_path)

    clone_command = "git clone https://github.com/curl/curl.git"
    os.system(clone_command)

    inside_folder_path = curl_path + "/curl"
    chdir(inside_folder_path)

    make_command = "./buildconf && ./configure && make"
    os.system(make_command)


def json():
    json_path = workspace_path / "external/json"
    chdir(json_path)

    clone_command = "git clone https://github.com/andrewkatson/json.git"
    os.system(clone_command)


def randomx():
    randomx_path = workspace_path / "external"
    chdir(randomx_path)

    clone_command = "git clone https://github.com/tevador/RandomX.git"
    os.system(clone_command)

    make_command = "cd RandomX && mkdir build && cd build && cmake -DARCH=native .. && make"
    os.system(make_command)

    chdir(randomx_path)

    mv_command = "mv RandomX randomx"
    os.system(mv_command)


def bigint():
    bigint_path = workspace_path / "external"
    chdir(bigint_path)

    clone_command = "git clone https://github.com/kasparsklavins/bigint.git"
    os.system(clone_command)


def import_dependencies():
    chdir(workspace_path)
    wolfssl()
    chdir(workspace_path)
    curl()
    chdir(workspace_path)
    json()
    chdir(workspace_path)
    randomx()
    chdir(workspace_path)
    bigint()


def import_dependencies_win():
    chdir(workspace_path)
    wolfssl_win()
    chdir(workspace_path)
    json()
    chdir(workspace_path)
    bigint()
    chdir(workspace_path)


def main():
    global workspace_path
    workspace_path = workspace_path_finder.find_workspace_path()
    if sys.platform == "linux":
        print("Importing dependencies \n\n\n\n\n")
        import_dependencies()
    elif sys.platform == "msys":
        print("Importing dependencies Win \n\n\n\n\n")
        import_dependencies_win()


main()
