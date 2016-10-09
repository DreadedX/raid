//----------------------------------------------
#include <iostream>

#include "raid/engine.h"
#include "raid/graphics/opengl/opengl.h"
//----------------------------------------------
std::shared_ptr<raid::Engine> raid::Engine::engine;
//----------------------------------------------
raid::Engine::Engine() {
	// OpenGL should not be here
	graphics = std::make_shared<GraphicsManager>();
	file_manager = std::make_shared<FileManager>();
	resource = std::make_shared<ResourceManager>();
}
//----------------------------------------------
raid::Engine::~Engine() {
}
//----------------------------------------------
std::shared_ptr<raid::Engine> raid::Engine::instance() {
	if (engine == nullptr) {
		engine = std::shared_ptr<Engine>(new Engine());
	}

	return engine;
}
//----------------------------------------------
