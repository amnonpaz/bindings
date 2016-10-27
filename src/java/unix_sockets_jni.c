#include "unix_sockets_jni.h"
#include "client.h"

#include <string.h>

#define BUFFER_SIZE 2048
#define MESSAGE_MAX_SIZE 256

JNIEXPORT jint JNICALL Java_UnixSocket_open(JNIEnv *env, jobject obj,
                                             jstring jSocketName)
{
    char message[MESSAGE_MAX_SIZE];

    const char *socket_name = (*env)->GetStringUTFChars(env, jSocketName, NULL);
    int s = socket_open(socket_name);
    
    if (s <= 0) {
        jclass Exception = (*env)->FindClass(env, "java/lang/Exception");
        snprintf(message, MESSAGE_MAX_SIZE, "Couldn't open socket %s", socket_name);
        (*env)->ThrowNew(env, Exception, message);
    }

    (*env)->ReleaseStringUTFChars(env, jSocketName, socket_name);
    return (jint)s;
}

JNIEXPORT void JNICALL Java_UnixSocket_close(JNIEnv *env, jobject obj,
                                              jint jSocket)
{
    socket_close((int)jSocket);
}

JNIEXPORT jstring JNICALL Java_UnixSocket_read(JNIEnv *env, jobject obj,
                                                jint jSocket)
{
    char buffer[BUFFER_SIZE];
    int res;

    res = socket_read(jSocket, buffer, BUFFER_SIZE);
    if (res < 0) {
        jclass Exception = (*env)->FindClass(env, "java/lang/Exception");
        (*env)->ThrowNew(env, Exception, "Error reading from socket");
    } else {
        buffer[res-1] = '\0';
    }

    return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT void JNICALL Java_UnixSocket_write(JNIEnv *env, jobject obj,
                                              jint jSocket, jstring jBuffer)
{
    const char *buffer = (*env)->GetStringUTFChars(env, jBuffer, NULL);
    int res = socket_write((int)jSocket, buffer, strlen(buffer)+1);
    (*env)->ReleaseStringUTFChars(env, jBuffer, buffer);

    if (res <= 0) {
        jclass Exception = (*env)->FindClass(env, "java/lang/Exception");
        (*env)->ThrowNew(env, Exception, "Error writing to socket");
    }
}
