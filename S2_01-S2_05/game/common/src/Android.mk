LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL_image/include $(LOCAL_PATH)/../SDL_ttf/include $(LOCAL_PATH)/../SDL_mixer/include $(LOCAL_PATH)/../include

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp \
	res/debugfont.cpp \
	core/Game.cpp \
	core/Actors.cpp \
	core/InputHandler.cpp \
	core/SaveMgr.cpp \
	core/TextMgr.cpp \
	core/io.cpp \
	actors/A_Camera.cpp \
	actors/A_Chest.cpp \
	actors/A_Credits.cpp \
	actors/A_Inter.cpp \
	actors/A_InvMgr.cpp \
	actors/A_Joueur.cpp \
	actors/A_Map.cpp \
	actors/A_Mouse.cpp \
	actors/A_PickupObj.cpp \
	actors/A_PNJ.cpp \
	actors/A_PNJ_1.cpp \
	actors/A_PNJ_2.cpp \
	actors/A_PNJ_3.cpp \
	actors/A_PNJ_Max.cpp \
	actors/A_PNJ_Alex.cpp \
	actors/A_TextBox.cpp \
	actors/A_Titlescreen.cpp \
	actors/A_AndroidCtrl.cpp \
	actors/A_GUI.cpp \
	actors/A_Cactus.cpp
	

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
