#include "convert.h"

JNIEXPORT void JNICALL Java_com_keiros_example_strings_Convert_something(JNIEnv * env, jobject obj, jstring str) {
  const char *convertedValue = (env)->GetStringUTFChars(str, nullptr);

  std::string ipCopy = std::string(convertedValue);

  std::cout << ipCopy << std::endl;

  env->ReleaseStringUTFChars(str, convertedValue);
}

int main() {

  char *example = "This";

  std::string ex = std::string(example);

  std::cout << ex << std::endl;

  return 0;
}