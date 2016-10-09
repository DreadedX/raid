//----------------------------------------------
#include <iostream>

#include "raid/graphics/opengl/opengl.h"
//----------------------------------------------
void error_callback(int error, const char *description) {

	std::cout << description << " (" << error << ")\n";
}
//----------------------------------------------
void key_callback(GLFWwindow* window, int key, int, int action, int) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
//----------------------------------------------
void raid::OpenGL::create_window(int width, int height, std::string title) {

	if(!glfwInit()) {
		std::cout << "Failed to initialize glfw\n";
		/// @todo Create a better way to exit
		exit(-1);
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Force myself to make sure the engine supports resizing
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create window\n";
		/// @todo Create a better way to exit
		exit(-1);
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowPos(window, (mode->width - width)/2, (mode->height - height)/2);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		std::cout << "Failed to initialize glew: " << glewGetErrorString(glew_status) << '\n';
	}

	glfwSwapInterval(0);
	glfwSetKeyCallback(window, key_callback);

	if (glGetError() != GL_NO_ERROR) {
		std::cout << "Something went wrong\n";
	}
}
//----------------------------------------------
inline bool raid::OpenGL::should_window_close() {

	return glfwWindowShouldClose(window);
}
//----------------------------------------------
inline void raid::OpenGL::poll_events() {

	glfwPollEvents();
}
//----------------------------------------------
inline void raid::OpenGL::swap_buffers() {

	glfwSwapBuffers(window);
}
//----------------------------------------------
