//----------------------------------------------
#include <iostream>

#include "raid/graphics/opengl/opengl.h"

#include "logger.h"
//----------------------------------------------
void error_callback(int error, const char *description) {

	warning << description << " (" << error << ")\n";
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
		warning << "Failed to initialize glfw\n";
		/// @todo This should be an exception
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
		warning << "Failed to create window\n";
		/// @todo This should be an exception
		exit(-1);
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowPos(window, (mode->width - width)/2, (mode->height - height)/2);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		warning << "Failed to initialize glew: " << glewGetErrorString(glew_status) << '\n';
		/// @todo This should be an exception
		exit(-1);
	}

	glfwSwapInterval(0);
	glfwSetKeyCallback(window, key_callback);

	if (glGetError() != GL_NO_ERROR) {
		warning << "Something went wrong\n";
		/// @todo This should be an exception
		exit(-1);
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
