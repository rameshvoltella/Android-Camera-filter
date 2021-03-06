/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "apFFMuxer.h"
#include "autolock.h"
#define LOG_TAG "easymuxer"
#include "pplog.h"
#include <pthread.h>

/* Header for class com_gotye_sdk_FFMuxer */

#ifndef _Included_com_gotye_sdk_FFMuxer
#define _Included_com_gotye_sdk_FFMuxer
#ifdef __cplusplus
extern "C" {
#endif

#include "andsysutil.h"

struct fields_t {
	jfieldID    handle; // for save encoder handle
	jfieldID	listener; // for save listener handle
	jmethodID   post_event;
};

static pthread_mutex_t sLock;
static fields_t muxer_fields;

static apFFMuxer* getMuxer(JNIEnv* env, jobject thiz);

static apFFMuxer* setMuxer(JNIEnv* env, jobject thiz, apFFMuxer *muxer);

static int convert_jstring(JNIEnv* env, char *des_str, int* len, jstring str);

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeOpen
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_gotye_sdk_FFMuxer_nativeOpen
  (JNIEnv *env, jobject thiz, jstring url)
{
	PPLOGI("nativeOpen()");
	
	pthread_mutex_init(&sLock, NULL);

	jclass clazzEncoder = env->FindClass("com/gotye/sdk/FFMuxer");
	if (clazzEncoder == NULL) {
		PPLOGE("muxer is null, FFMuxer failed");
		return JNI_FALSE;
	}

	muxer_fields.handle = env->GetFieldID(clazzEncoder, "mHandle", "J");

	apFFMuxer *muxer = new apFFMuxer();
	setMuxer(env, thiz, muxer);

	char str_url[1024] = {0};
	int str_len = sizeof(str_url);
	convert_jstring(env, str_url, &str_len, url);

	return muxer->open(str_url);
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeAddVideo
 * Signature: (IIII)I
 */
JNIEXPORT jint JNICALL Java_com_gotye_sdk_FFMuxer_nativeAddVideo
  (JNIEnv *env, jobject thiz, jint width, jint height, jint framerate, jint bitrate)
{
	AutoLock lock(&sLock);

	PPLOGD("nativeAddVideo()");

	apFFMuxer *muxer = getMuxer(env, thiz);
	return muxer->add_video(width, height, framerate, bitrate);
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeAddAudio
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_com_gotye_sdk_FFMuxer_nativeAddAudio
  (JNIEnv *env, jobject thiz, jint sample_rate, jint channels, jint bitrate)
{
	AutoLock lock(&sLock);

	PPLOGD("nativeAddAudio()");

	apFFMuxer *muxer = getMuxer(env, thiz);
	return muxer->add_audio(sample_rate, channels, bitrate);
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeGetBitrate(kbps)
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_gotye_sdk_FFMuxer_nativeGetBitrate
  (JNIEnv *env, jobject thiz)
{
	AutoLock lock(&sLock);

	apFFMuxer *muxer = getMuxer(env, thiz);
	return muxer->get_bps();
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeGetBufferingSize(byte)
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_gotye_sdk_FFMuxer_nativeGetBufferingSize
  (JNIEnv *env, jobject thiz)
{
	AutoLock lock(&sLock);

	apFFMuxer *muxer = getMuxer(env, thiz);
	return muxer->get_buffering_size();
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeSetSpsAndPps
 * Signature: ([B)Z
 */
JNIEXPORT jboolean JNICALL Java_com_gotye_sdk_FFMuxer_nativeSetMetaData
  (JNIEnv *env, jobject thiz, jint stream_index, jstring key, jstring value)
{
	AutoLock lock(&sLock);

	PPLOGI("nativeSetMetaData");

	char str_key[1024] = {0};
	char str_value[1024] = {0};
	int str_len;
	
	str_len = sizeof(str_key);
	convert_jstring(env, str_key, &str_len, key);
	str_len = sizeof(str_value);
	convert_jstring(env, str_value, &str_len, value);
	apFFMuxer *muxer = getMuxer(env, thiz);
	return muxer->set_metadata(stream_index, str_key, str_value);
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeSetSpsAndPps
 * Signature: ([B)Z
 */
JNIEXPORT jboolean JNICALL Java_com_gotye_sdk_FFMuxer_nativeSetSpsAndPps
  (JNIEnv *env, jobject thiz, jbyteArray sps_pps)
{
	AutoLock lock(&sLock);

	PPLOGI("nativeSetSpsAndPps");

	apFFMuxer *muxer = getMuxer(env, thiz);
	jbyte* p_data = env->GetByteArrayElements(sps_pps, NULL);
	jsize len = env->GetArrayLength(sps_pps);

	bool ret = muxer->set_sps_pps((uint8_t *)p_data, len);
	env->ReleaseByteArrayElements(sps_pps, p_data, 0);
	return ret;
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeAddData
 * Signature: (Z[BIIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_gotye_sdk_FFMuxer_nativeWriteFrame
  (JNIEnv *env, jobject thiz, jboolean isVideo, jbyteArray data, jint start, jint byteCount, jlong timestamp/* msec */)
{
	AutoLock lock(&sLock);

	PPLOGD("nativeWriteFrame");

	apFFMuxer *muxer = getMuxer(env, thiz);
	jbyte* p_data = env->GetByteArrayElements(data, NULL);
	//jsize data_size = env->GetArrayLength(data);

	bool ret = muxer->write_frame(isVideo, (uint8_t *)p_data + start, byteCount, timestamp);
	env->ReleaseByteArrayElements(data, p_data, JNI_ABORT);
	return ret;
}

/*
 * Class:     com_gotye_sdk_FFMuxer
 * Method:    nativeClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_gotye_sdk_FFMuxer_nativeClose
  (JNIEnv *env, jobject thiz)
{
	PPLOGI("nativeClose()");

	apFFMuxer *muxer = getMuxer(env, thiz);
	if (!muxer)
		return;

	muxer->close();
	delete muxer;
	muxer = NULL;

	setMuxer(env, thiz, NULL);
	pthread_mutex_destroy(&sLock);

	PPLOGI("nativeClose() done");
}

static apFFMuxer* getMuxer(JNIEnv* env, jobject thiz)
{
	long handle = env->GetLongField(thiz, muxer_fields.handle);
	return (apFFMuxer *)handle;
}

static apFFMuxer* setMuxer(JNIEnv* env, jobject thiz, apFFMuxer* muxer)
{
	long old = env->GetLongField(thiz, muxer_fields.handle);
	env->SetLongField(thiz, muxer_fields.handle, (jlong)muxer);
	return (apFFMuxer *)old;
}

static int convert_jstring(JNIEnv* env, char *des_str, int* len, jstring str)
{
	const char *nativeString = env->GetStringUTFChars(str, 0);     
	and_sysutil_strcpy(des_str, nativeString, *len);
	env->ReleaseStringUTFChars(str, nativeString);
	
	return 0;
}

#ifdef __cplusplus
}
#endif
#endif
