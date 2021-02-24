#include <jni.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <android/log.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <string.h>

#define TAG    "myhello-jni-test" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型

char *write_ptr = nullptr;
static const char *const kClassDocScanner = "com/example/mmap_lib/LogHelper";

static char *openMMap(int buffer_fd, size_t buffer_size);

static jlong
initNative(JNIEnv *env, jclass instance, jstring buffer_path_, jint capacity, jstring log_path_) {
    const char *buffer_path = env->GetStringUTFChars(buffer_path_, nullptr);
    const char *log_path = env->GetStringUTFChars(log_path_, nullptr);
    size_t buffer_size = static_cast<size_t>(capacity);
    int buffer_fd = open(buffer_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    char *buffer_ptr = openMMap(buffer_fd, buffer_size);
    env->ReleaseStringUTFChars(buffer_path_, buffer_path);
    env->ReleaseStringUTFChars(log_path_, log_path);
    write_ptr = buffer_ptr;
    return reinterpret_cast<long>(buffer_ptr);
}

static char *openMMap(int buffer_fd, size_t buffer_size) {
    char *map_ptr = nullptr;
    if (buffer_fd != -1) {
        // 根据 buffer size 调整 buffer 文件大小
        ftruncate(buffer_fd, static_cast<int>(buffer_size));
        lseek(buffer_fd, 0, SEEK_SET);
        map_ptr = (char *) mmap(0, buffer_size, PROT_WRITE | PROT_READ, MAP_SHARED, buffer_fd, 0);
        if (map_ptr == MAP_FAILED) {
            map_ptr = nullptr;
        }
    }
    return map_ptr;
}

static void writeNative(JNIEnv *env, jobject instance, jlong ptr, jstring log_) {
    const char *log = env->GetStringUTFChars(log_, 0);
    jsize log_len = env->GetStringUTFLength(log_);
    memcpy(reinterpret_cast<void *>(write_ptr), log, log_len);
    write_ptr = write_ptr + log_len;


    env->ReleaseStringUTFChars(log_, log);
    LOGD("---lin---> hello world write native");
}

static JNINativeMethod gMethods[] = {
        {
                "initNative",
                "(Ljava/lang/String;ILjava/lang/String;)J",
                (void *) initNative
        },
        {
                "writeNative",
                "(JLjava/lang/String;)V",
                (void *) writeNative
        },
};

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_FALSE;
    }
    jclass classDocScanner = env->FindClass(kClassDocScanner);
    if (env->RegisterNatives(classDocScanner, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) <
        0) {
        return JNI_FALSE;
    }
    return JNI_VERSION_1_4;
}