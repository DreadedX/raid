#pragma once
//----------------------------------------------
#include <memory>

#include "raid/graphics/manager.h"
#include "raid/io/manager.h"
#include "raid/resource.h"

#include "logger.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class Engine {
		public:
			~Engine() { debug << "Engine destructed\n"; }
			// static std::shared_ptr<Engine> instance();
			static Engine& instance();

			Engine(Engine const&) = delete;
			void operator=(Engine const&) = delete;

			// std::shared_ptr<GraphicsManager> get_graphics() { return graphics; }
			// std::shared_ptr<FileManager> get_file_manager() { return file_manager; }
			// std::shared_ptr<ResourceManager> get_resource() { return resource; }
			GraphicsManager& get_graphics() { return graphics; }
			FileManager& get_file_manager() { return file_manager; }
			ResourceManager& get_resource() {return resource; }
		protected:
			Engine() { 
				debug << "Engine constructed!\n"; 
			}
		private:
			// std::shared_ptr<GraphicsManager> graphics = std::make_shared<GraphicsManager>();
			// std::shared_ptr<FileManager> file_manager = std::make_shared<FileManager>();
			// std::shared_ptr<ResourceManager> resource = std::make_shared<ResourceManager>();
			GraphicsManager graphics;
			FileManager file_manager;
			ResourceManager resource;
	};
	//----------------------------------------------
}
