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

Run `bazel run :configure`

and 

Run `bazel run :configure_win`

## Building 

### Linux 

Just use `bazel build target`

### Windows

Just use `bazel build target`