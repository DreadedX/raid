#pragma once
//----------------------------------------------
#include <iostream>
#include <cassert>

#include <android_native_app_glue.h>

#include "raid/platform/opengl_base/opengl_base.h"

#include <EGL/egl.h>
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class Android : public OpenGLBase {

		public:
			Android(android_app* app);
			/// Create an opengl window
			void create_window(int width, int height, std::string title) override;
			inline void terminate() override;
			inline bool should_window_close() override;
			inline bool has_context() override;
			inline void poll_events() override;
			inline void swap_buffers() override;

		private:
			struct android_app* app;

			EGLDisplay display;
			EGLSurface surface;
			EGLContext context;
			int32_t width;
			int32_t height;
	};
	//----------------------------------------------
}
