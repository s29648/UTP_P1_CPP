/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_utpp1_Main */

#ifndef _Included_com_example_utpp1_Main
#define _Included_com_example_utpp1_Main
#ifdef __cplusplus
extern "C" {
#endif
#undef com_example_utpp1_Main_ROWS
#define com_example_utpp1_Main_ROWS 6L
#undef com_example_utpp1_Main_COLS
#define com_example_utpp1_Main_COLS 7L
    /*
     * Class:     com_example_utpp1_Main
     * Method:    resetGame
     * Signature: ()V
     */
    JNIEXPORT void JNICALL Java_com_example_utpp1_Main_resetGame
      (JNIEnv *, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    dropToken
     * Signature: (ILjavafx/scene/shape/Circle;)Z
     */
    JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_dropToken
      (JNIEnv *, jobject, jint, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    getCurrentPlayer
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getCurrentPlayer
      (JNIEnv *, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    checkWinner
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_checkWinner
      (JNIEnv *, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    isADraw
     * Signature: ()Z
     */
    JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_isADraw
      (JNIEnv *, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    getLastMoveRow
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getLastMoveRow
      (JNIEnv *, jobject);

    /*
     * Class:     com_example_utpp1_Main
     * Method:    getTokenPosition
     * Signature: (Ljavafx/scene/shape/Circle;)Ljava/awt/Point;
     */
    JNIEXPORT jobject JNICALL Java_com_example_utpp1_Main_getTokenPosition
      (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif