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
			void terminate() override;
			bool should_window_close() override;
			bool has_context() override;
			void poll_events() override;
			void swap_buffers() override;

			virtual std::string get_storage_path() { return "."; }

		private:
			/// Raw pointer to window
			GLFWwindow* window;
	};
	//----------------------------------------------
}
