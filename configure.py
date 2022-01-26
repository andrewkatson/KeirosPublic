# Configuration script to set up the dependencies for KeirosPublic automatically.

import os

# Need to explicitly set this.
workspace_path = "/home/andrew/KeirosPublic"

# A workspace path that works if not suco on EC2
try:
    possible_workspace_path = os.environ["HOME"] + "/KeirosPublic"
    if os.path.exists(possible_workspace_path):
        workspace_path = possible_workspace_path
except Exception as e:
    print(e)
    print("The HOME variable does not point to the directory")

# A workspace path that works in sudo on EC2
try:
    possible_workspace_path = "home/" + os.environ["SUDO_USER"] + "/KeirosPublic"

    if os.path.exists(possible_workspace_path):
        workspace_path = possible_workspace_path
except Exception as e:
    print(e)
    print("Not on an EC2 using sudo")


def wolfssl():
    wolfssl_path = workspace_path + "/external/wolfssl"
    os.chdir(wolfssl_path)

    clone_command = "git clone https://github.com/andrewkatson/wolfssl.git"
    os.system(clone_command)

    inside_folder_path = wolfssl_path + "/wolfssl"
    os.chdir(inside_folder_path)

    build_command = "./configure --enable-ipv6 --enable-blake2 --enable-blake2s --enable-keygen --enable-certgen --enable-certreq --enable-ed25519 --enable-eccencrypt && make && sudo make install && mkdir build && cp /usr/local/lib/libwolfssl* " + inside_folder_path + "/build"
    os.system(build_command)


def main():
    wolfssl()
    os.chdir(workspace_path)


main()
