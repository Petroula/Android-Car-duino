APP_OPTIM := release
APP_STL := gnustl_shared
APP_CPPFLAGS := -frtti -fexceptions -std=c++11 -O3 -Wno-deprecated -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wpacked -pipe
NDK_TOOLCHAIN_VERSION := 4.8
APP_ABI := mips armeabi armeabi-v7a