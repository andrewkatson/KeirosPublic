# KeirosPublic

## Description
Any keiros protos that need to be shared publically so that other people can use them. 

## Install 
* Bazel: https://bazel.build/

### Linux

Run `bazel run :configure`

### Windows
* Need to set JAVA_HOME in msys. Mine is ```/c/'Program Files'/Java/jdk-10.0.2```
* Need to build with ```--javabase=@bazel_tools//tools/jdk:remote_jdk11```

Run `bazel run :configure_win (in command prompt)`
Run `bazel run :configure (in msys2)`

Open `external/wolfssl/wolfssl/wolfssl64.sln` in Visual Studio. Choose `x86` and then select `Build > Build Solution` 

## Building 

### Linux 

Just use `bazel build :target`

### Windows

Just use `bazel build :target`
