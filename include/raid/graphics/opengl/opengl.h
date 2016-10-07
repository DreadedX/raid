#pragma once
//----------------------------------------------
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "raid/graphics/kit.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGLImpl : public GraphicsKit::Impl {

		public:
			/// Create an opengl window
			void create_window(int width, int height, std::string title) override;
			inline bool should_window_close() override;
			inline void poll_events() override;
			inline void swap_buffers() override;

		private:
			/// Raw pointer to window
			GLFWwindow* window;
	};
	//----------------------------------------------
}
