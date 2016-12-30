#pragma once
//----------------------------------------------
#include <iostream>
#include <cassert>

#include "raid/platform/opengl_base/opengl_base.h"

#include <GLFW/glfw3.h>
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class GLFW : public OpenGLBase {

		public:
			/// Create an opengl window
			void create_window(int width, int height, std::string title) override;
			inline void terminate() override;
			inline bool should_window_close() override;
			inline bool has_context() override;
			inline void poll_events() override;
			inline void swap_buffers() override;

		private:
			/// Raw pointer to window
			GLFWwindow* window;
	};
	//----------------------------------------------
}
