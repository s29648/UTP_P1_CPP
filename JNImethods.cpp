//
// Created by Darya Sharnevich on 02/11/2024.
//

#include "com_example_utpp1_Main.h"
#include "ConnectFourGame.h"


JNIEXPORT void JNICALL Java_com_example_utpp1_Main_resetGame(JNIEnv *env, jobject obj) {
    //delete global references to tokens that are not used anymore
    for (auto &[tokenKey, positionVal]: tokenPositionMap) {
        env->DeleteGlobalRef(tokenKey);
    }
    //clear the tokenPositionMap before new game
    tokenPositionMap.clear();

    ConnectFourGame::getInstance().resetGame();
}

JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_dropToken(JNIEnv *env, jobject obj, const jint column,
                                                                 jobject token) {
    ConnectFourGame &game = ConnectFourGame::getInstance();
    return static_cast<jboolean>(game.dropToken(static_cast<int>(column), token, env));
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getCurrentPlayer(JNIEnv *env, jobject obj) {
    return ConnectFourGame::getInstance().getCurrentPlayer();
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_checkWinner(JNIEnv *env, jobject obj) {
    return static_cast<jint>(ConnectFourGame::getInstance().checkWinner());
}

JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_isADraw(JNIEnv *, jobject) {
    return static_cast<jboolean>(ConnectFourGame::getInstance().isADraw());
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getLastMoveRow(JNIEnv *env, jobject obj) {
    return ConnectFourGame::getInstance().getLastMoveRow();
}

JNIEXPORT jobject JNICALL Java_com_example_utpp1_Main_getTokenPosition(JNIEnv *env, jobject obj, jobject token) {
    // iterate over tokenPositionMap:
    for (const auto &[tokenKey, positionVal]: tokenPositionMap) {
        //compare two jobject references to determine if they refer to the same Java object:
        if (env->IsSameObject(tokenKey, token)) {
            Point position = positionVal;

            //retrieve a reference to the Java Point class:
            jclass pointClass = env->FindClass("java/awt/Point");
            // find the constructor ("<init>") of Point class
            // that takes two integers as arguments (II) and returns void (V):
            jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(II)V");

            //create a new instance of Java Point class with position.x and position.y arguments, from C++ Point struct
            jobject javaPoint = env->NewObject(pointClass, constructor, position.x, position.y);

            return javaPoint;
        }
    }

    return nullptr;
}
