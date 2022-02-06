//
// Created by andrew on 2/6/22.
//

#include <jni.h>
#include <iostream>
#include <string>

extern "C" {
JNIEXPORT void JNICALL Java_com_keiros_example_strings_Convert_something(JNIEnv *, jobject, jstring);
}

#ifndef KEIROSPUBLIC_CONVERT_H
#define KEIROSPUBLIC_CONVERT_H

class Convert {

};

#endif //KEIROSPUBLIC_CONVERT_H
