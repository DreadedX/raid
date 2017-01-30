#pragma once

#ifdef ANDROID
	#include "raid/platform/android/android.h"
	#define PLATFORM_IMPL Android
	#define PLATFORM_ARGS app
	#define PLATFORM_GL_VERSION "300 es"
	#define ENTRY void android_main(android_app* app)
	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Engine", __VA_ARGS__))
	#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Engine", __VA_ARGS__))
#else
	#include "raid/platform/glfw/glfw.h"
	#define PLATFORM_IMPL GLFW
	// #define PLATFORM_IMPL DummyPlatform
	#define PLATFORM_ARGS
	#define PLATFORM_GL_VERSION "330 core"
	#define ENTRY int main()
#endif
