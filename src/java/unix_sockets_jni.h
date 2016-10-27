#ifndef __LINUX_SOCKETS_JNI_H__
#define __LINUX_SOCKETS_JNI_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_LinuxSocket_open(JNIEnv *env, jobject obj,
                                             jstring jSocketName);
JNIEXPORT void JNICALL Java_LinuxSocket_close(JNIEnv *env, jobject obj,
                                              jint jSocket);
JNIEXPORT jstring JNICALL Java_LinuxSocket_read(JNIEnv *env, jobject obj,
                                                jint jSocket);
JNIEXPORT jint JNICALL Java_LinuxSocket_write(JNIEnv *env, jobject obj,
                                              jint jSocket, jstring jBuffer);

#ifdef __cplusplus
}
#endif

#endif /* __LINUX_SOCKETS_JNI_H__ */
