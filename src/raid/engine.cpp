//----------------------------------------------
#include <iostream>

#include "raid/engine.h"
#include "raid/graphics/opengl/opengl.h"
//----------------------------------------------
// std::shared_ptr<raid::Engine> raid::Engine::engine;
//----------------------------------------------
// std::shared_ptr<raid::Engine> raid::Engine::instance() {
// 	if (engine == nullptr) {
// 		engine = std::shared_ptr<Engine>(new Engine());
// 	}
//
// 	return engine;
// }
//----------------------------------------------
raid::Engine& raid::Engine::instance() {
	static Engine engine;
	return engine;
}
//----------------------------------------------
