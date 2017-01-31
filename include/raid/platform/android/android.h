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
			void terminate() override;
			bool should_window_close() override;
			bool has_context() override;
			void poll_events() override;
			void swap_buffers() override;

			bool is_pressed(int x, int y, int width, int height) override;

			std::string get_storage_path() override { return app->activity->externalDataPath; }

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
