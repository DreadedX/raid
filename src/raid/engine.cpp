//----------------------------------------------
#include <iostream>

#include "raid/engine.h"
//----------------------------------------------
raid::Engine::Engine() { 
	debug << "Engine constructed!\n"; 
}
//----------------------------------------------
raid::Engine& raid::Engine::instance() {
	static Engine engine;
	return engine;
}
//----------------------------------------------
