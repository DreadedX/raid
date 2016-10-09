#pragma once
//----------------------------------------------
#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "raid/graphics/manager.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGL : public GraphicsManager::Impl {

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
