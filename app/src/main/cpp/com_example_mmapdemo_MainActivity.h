//
// Created by linsir on 2021/2/9.
//

#ifndef MMAPDEMO_COM_EXAMPLE_MMAPDEMO_MAINACTIVITY_H
#define MMAPDEMO_COM_EXAMPLE_MMAPDEMO_MAINACTIVITY_H

#endif //MMAPDEMO_COM_EXAMPLE_MMAPDEMO_MAINACTIVITY_H



JNIEXPORT jint JNICALL Java_com_test_jcit_helloworld_MainActivity_addCalc
  (JNIEnv *env, jclass jobj, jint ja, jint jb) {
  return ja + jb;
}