#pragma once
//----------------------------------------------
#include <iostream>
#include <cassert>

#ifdef ANDROID
	#include <GLES3/gl3.h>
#else
	#include <GL/glew.h>
#endif

#include "raid/platform/proxy.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGLBase : public PlatformProxy::Impl {

		public:
			// uint load_shader();
			void test_setup();
			void test_render();
	};
	//----------------------------------------------
}
